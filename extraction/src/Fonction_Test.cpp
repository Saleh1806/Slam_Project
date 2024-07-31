// #include <sstream>
// #include <fstream>
// #include <stdexcept>
// #include <queue>
// #include <set>
// #include <list>
// #include <iterator>
// #include <map>
// #include <vector>
// #include <utility>
// #include <algorithm>
// #include <string>

// #include "karto_sdk/Mapper.h"


// using namespace karto;

//   kt_bool m_Initialized;
//   kt_bool m_Deserialized;

//   ScanMatcher * m_pSequentialScanMatcher;

//   MapperSensorManager * m_pMapperSensorManager;

//   MapperGraph * m_pGraph;
//   ScanSolver * m_pScanOptimizer;
//   LocalizationScanVertices m_LocalizationScanVertices;
//     Parameter<kt_double> * m_pCorrelationSearchSpaceDimension;


//   Parameter<kt_double> * m_pLinkScanMaximumDistance;

//   /**
//    * Enable/disable loop closure.
//    * Default is enabled.
//    */
//   Parameter<kt_bool> * m_pDoLoopClosing;

//   /**
//    * Scans less than this distance from the current position will be considered for a match
//    * in loop closure.
//    * Default value is 4.0 meters.
//    */
//   Parameter<kt_double> * m_pLoopSearchMaximumDistance;

//   /**
//    * When the loop closure detection finds a candidate it must be part of a large
//    * set of linked scans. If the chain of scans is less than this value we do not attempt
//    * to close the loop.
//    * Default value is 10.
//    */
//   Parameter<kt_int32u> * m_pLoopMatchMinimumChainSize;

//   /**
//    * The co-variance values for a possible loop closure have to be less than this value
//    * to consider a viable solution. This applies to the coarse search.
//    * Default value is 0.16.
//    */
//   Parameter<kt_double> * m_pLoopMatchMaximumVarianceCoarse;

//   /**
//    * If response is larger then this, then initiate loop closure search at the coarse resolution.
//    * Default value is 0.7.
//    */
//   Parameter<kt_double> * m_pLoopMatchMinimumResponseCoarse;

//   /**
//    * If response is larger then this, then initiate loop closure search at the fine resolution.
//    * Default value is 0.7.
//    */
//   Parameter<kt_double> * m_pLoopMatchMinimumResponseFine;



//   /**
//    * The resolution (size of a grid cell) of the correlation grid.
//    * Default value is 0.01 meters.
//    */
//   Parameter<kt_double> * m_pCorrelationSearchSpaceResolution;

//   /**
//    * The point readings are smeared by this value in X and Y to create a smoother response.
//    * Default value is 0.03 meters.
//    */
//   Parameter<kt_double> * m_pCorrelationSearchSpaceSmearDeviation;




//   std::vector<MapperListener *> m_Listeners;

//   void Initialize(kt_double rangeThreshold)
// {
//   if (m_Initialized) {
//     return;
//   }
//   // create sequential scan and loop matcher, update if deserialized

//   if (m_pSequentialScanMatcher) {
//     delete m_pSequentialScanMatcher;
//   }
//   m_pSequentialScanMatcher = ScanMatcher::Create(this,
//     m_pCorrelationSearchSpaceDimension->GetValue(),
//     m_pCorrelationSearchSpaceResolution->GetValue(),
//     m_pCorrelationSearchSpaceSmearDeviation->GetValue(),
//     rangeThreshold);
//   assert(m_pSequentialScanMatcher);

//   if (m_Deserialized) {
//     m_pMapperSensorManager->SetRunningScanBufferSize(m_pScanBufferSize->GetValue());
//     m_pMapperSensorManager->SetRunningScanBufferMaximumDistance(m_pScanBufferMaximumScanDistance->GetValue());

//     m_pGraph->UpdateLoopScanMatcher(rangeThreshold);
//   } else {
//     m_pMapperSensorManager = new MapperSensorManager(m_pScanBufferSize->GetValue(),
//       m_pScanBufferMaximumScanDistance->GetValue());

//     m_pGraph = new MapperGraph(this, rangeThreshold);
//   }

//   m_Initialized = true;
// }


// kt_bool Process(LocalizedRangeScan* pScan, Matrix3* covariance)
// {
//     static int it = 0;  // Déclaration de it comme variable statique

//     // Chemins des fichiers pour la sérialisation
//     std::string inputIterationFilePath = "/tmp/test/input_iteration_" + std::to_string(it) + ".txt";
//     std::string outputIterationFilePath = "/tmp/test/output_iteration_" + std::to_string(it) + ".txt";
    
//     // Sérialiser les données d'entrée avant traitement
//     std::ofstream inputFile(inputIterationFilePath);
//     if (inputFile.is_open()) {
//         inputFile << *pScan << "\n" << *covariance << "\n";
//         inputFile.close();
//     } else {
//         std::cerr << "Erreur lors de l'ouverture du fichier " << inputIterationFilePath << " pour la sauvegarde des données d'entrée." << std::endl;
//         return false; // Return error code
//     }

//     if (pScan != NULL) {
//         karto::LaserRangeFinder* pLaserRangeFinder = pScan->GetLaserRangeFinder();

//         // Validate scan
//         if (pLaserRangeFinder == NULL || pScan == NULL || pLaserRangeFinder->Validate(pScan) == false) {
//             return false;
//         }

//         if (m_Initialized == false) {
//             // Initialize mapper with range threshold from device
//             Initialize(pLaserRangeFinder->GetRangeThreshold());
//         }

//         // Get last scan
//         LocalizedRangeScan* pLastScan = m_pMapperSensorManager->GetLastScan(pScan->GetSensorName());

//         // Update scans corrected pose based on last correction
//         if (pLastScan != NULL) {
//             Transform lastTransform(pLastScan->GetOdometricPose(), pLastScan->GetCorrectedPose());
//             pScan->SetCorrectedPose(lastTransform.TransformPose(pScan->GetOdometricPose()));
//         }

//         // Test if scan is outside minimum boundary or if heading is larger than minimum heading
//         if (!HasMovedEnough(pScan, pLastScan)) {
//             return false;
//         }

//         Matrix3 cov;
//         cov.SetToIdentity();

//         // Correct scan (if not first scan)
//         if (m_pUseScanMatching->GetValue() && pLastScan != NULL) {
//             Pose2 bestPose;
//             m_pSequentialScanMatcher->MatchScan(pScan,
//                 m_pMapperSensorManager->GetRunningScans(pScan->GetSensorName()),
//                 bestPose,
//                 cov);
//             pScan->SetSensorPose(bestPose);
//             if (covariance) {
//                 *covariance = cov;
//             }
//         }

//         // Add scan to buffer and assign id
//         m_pMapperSensorManager->AddScan(pScan);

//         if (m_pUseScanMatching->GetValue()) {
//             // Add to graph
//             m_pGraph->AddVertex(pScan);
//             m_pGraph->AddEdges(pScan, cov);

//             m_pMapperSensorManager->AddRunningScan(pScan);

//             if (m_pDoLoopClosing->GetValue()) {
//                 std::vector<Name> deviceNames = m_pMapperSensorManager->GetSensorNames();
//                 const_forEach(std::vector<Name>, &deviceNames)
//                 {
//                     m_pGraph->TryCloseLoop(pScan, *iter);
//                 }
//             }
//         }

//         m_pMapperSensorManager->SetLastScan(pScan);

//         // Sérialiser les données de sortie après traitement
//         std::ofstream outputFile(outputIterationFilePath);
//         if (outputFile.is_open()) {
//             outputFile << true << "\n" << *pScan << "\n" << *covariance << "\n";
//             outputFile.close();
//         } else {
//             std::cerr << "Erreur lors de l'ouverture du fichier " << outputIterationFilePath << " pour la sauvegarde des données de sortie." << std::endl;
//         }

//         it++; // Incrémenter le compteur d'itération
//         return true;
//     }

//     return false;
// }

