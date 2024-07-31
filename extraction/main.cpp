#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <codecvt>
#include <locale>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "karto_sdk/Mapper.h"



// Fonction pour convertir std::wstring en std::string en utilisant UTF-8
std::string toUtf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

// Fonction pour charger LocalizedRangeScan à partir d'un fichier texte avec Boost.Serialization
void loadLocalizedRangeScan(const std::string& filePath, karto::LocalizedRangeScan& scan) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::cout << "Ouverture du fichier : " << filePath << std::endl;


    
    try {
        boost::archive::xml_iarchive ia(inputFile);
	printf("yoo \n");        
        ia >> BOOST_SERIALIZATION_NVP(scan);
    } catch (const std::exception& e) {
        std::cerr << "Erreur pendant la désérialisation de scan 2 : " << e.what() << std::endl;
        throw;
    }

    inputFile.close();
    std::cout << "Fermeture du fichier : " << filePath << std::endl;
}

// Fonction pour charger Matrix à partir d'un fichier texte avec Boost.Serialization
void loadMatrix(const std::string& filePath, karto::Matrix3& matrix) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::cout << "Ouverture du fichier : " << filePath << std::endl;

    try {
        boost::archive::xml_iarchive ia(inputFile);
        ia >> BOOST_SERIALIZATION_NVP(matrix);
    } catch (const std::exception& e) {
        std::cerr << "Erreur pendant la désérialisation de matrix: " << e.what() << std::endl;
        throw;
    }

    inputFile.close();
    std::cout << "Fermeture du fichier : " << filePath << std::endl;
}

int main(int argc, char **argv) {
    try {
        karto::Mapper mapper;

        // Chargement des données d'entrée depuis un fichier texte
        std::string inputFilePath = "/home/hp/ROS2_WS/src/slam_toolbox/lib/scan_data.xml";
        karto::LocalizedRangeScan scan;
        loadLocalizedRangeScan(inputFilePath, scan);
        exit(-1);
        
        
        std::string inputFilePath_Cov = "/home/hp/ROS2_WS/src/slam_toolbox/lib/covariance_data.xml";
        karto::Matrix3 covariance; // Utilisation de Matrix3
        loadMatrix(inputFilePath_Cov, covariance);

        // Appel de la méthode Process avec les données chargées
        bool processResult = mapper.Process(&scan, &covariance);

        // Vérifier que le processus a réussi
        if (!processResult) {
            std::cerr << "Process n'a pas réussi." << std::endl;
            return 1;
        }

        // Sérialisation des données de sortie dans un fichier texte
        std::string outputFilePath = "/home/hp/ROS2_WS/scan_data_extract.xml";
        std::ofstream outputFile(outputFilePath);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Cannot open file: " + outputFilePath);
        }

        boost::archive::xml_oarchive oa(outputFile);
        oa << BOOST_SERIALIZATION_NVP(processResult);
        oa << BOOST_SERIALIZATION_NVP(scan);
        oa << BOOST_SERIALIZATION_NVP(covariance);

        outputFile.close();

        std::cout << "Process terminé avec succès." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }
}

