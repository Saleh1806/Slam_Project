# Slam_Project

Ce projet est dans le cadre de notre projet de fin d'études de **Développement d’algorithmes de navigation autonome (SLAM) basés sur ROS 2 pour un robot mobile**. Dans ce projet nous avons utilisé la bibliothèuqe **slam_toolbox** et le capteur **RPLidar A2M12** . Et l'objectif était d'optimiser la cartographie et la localisation temps réel avec la programmation parallèle.

---

## Étapes
### 1. Installation
Installer ros2 humble
``` bash

cd ROS2_WS

colcon build --symlink-install

source ./install/setup.bash
```

### 1. Visualisation des données captées par le RPLidar  
Pour visualiser les données en temps réel, exécutez la commande suivante :  
```bash
ros2 launch rplidar_ros view_rplidar_a2m12_launch.py
```
### 2. Cartographie en temps réel  
Pour générer une carte en explorant l'environnement, utilisez la bibliothèque **slam_toolbox** :  
```bash
ros2 launch slam_toolbox online_async_launch.py
```

### 3. Analyse des données et optimisation  
Les fichiers `input_data.txt` et `output_data.txt` sont utilisés pour :  
- **Sérialiser les données** :
  - `input_data.txt` contient les données d'entrée nécessaires pour alimenter la méthode `Process` de la classe `Mapper`.  
  - `output_data.txt` contient les données de sortie générées après le traitement.  
- **Comparer les données** avant et après extraction hors ROS, pour garantir que les résultats restent identiques.  
- Une fois la vérification effectuée, il faudra se concentrer sur **l'optimisation en programmation parallèle** pour rendre la localisation et la cartographie en temps réel plus rapides.

---

## Extraction du SLAM hors ROS

On extrait les parties du SLAM qui assure la cartographie et la localisation en enlèvant leur dépendance à ROS. Ce travail est disponible dans le dossier **extraction**.  
- Les fichiers sérialisés (`input_data.txt` et `output_data.txt`) seront utilisées pour initialiser la classe `LocalizedRangeScan` dans la fonction  main.cpp du dossier extraction.  
- Notre objectif est de comparer les données de sortie avec celles générées par ROS. Si les résultats sont cohérents, on va optimiser la méthode **Process** de la classe Mapper qui s'occupe de la cartographie et de la localisation.

---

## Remarques
Pour faire ce projet avec la carte jetson nano il faut penser à utiliser le conteneur docker puisque la carte jetson nano ne supporte pas les nouvelles versions d'ubuntu qui supportent le ROS2. Et donc cette commande est important à savoir pour utiliser docker ave la jetson nano:

```bash
sudo docker run −−privileged −v /dev/ sensors / lidar :/ dev/ttyUSB0 \
 −v /home/ saliou / catkin_ws /:/ home/ catkin_ws −it ros bash
```
 • sudo: Indique que la commande est exécutée avec des privilèges administratifs.
 
 • docker run: Commande pour exécuter un nouveau conteneur Docker.
 
 •--privileged: Donne au conteneur un accès complet aux périphériques de l’hôte.
 
 •-v /dev/sensors/lidar:/dev/ttyUSB0: Montrelepériphériquematériel/dev/sensors/lidar
 
 de l’hôte dans le conteneur Docker sous /dev/ttyUSB0. Cela permet d’accéder au cap
teur LiDAR connecté à cet emplacement sur l’hôte.

 •-v /home/saliou/catkin_ws/:/home/catkin_ws: Montre le répertoire/home/saliou/catkin_ws/ de l’hôte dans le conteneur Docker sous /home/catkin_ws. Cela permet d’accéder aux fichiers et dossiers de ce répertoire à l’intérieur du conteneur.
 
 •-it: Cette option combine deux options :–-i, qui permet de garder le STDIN ouvert même si non connecté.–-t, qui alloue un pseudo-tty (Terminal) pour le conteneur. Cela permet d’interagir avec le terminal du conteneur une fois qu’il est lancé.
 
 • ros: C’est le nom de l’image Docker à partir de laquelle le conteneur est créé. Dans ce cas, c’est une image ROS (Robot Operating System).
 
 • bash: C’est la commande que nous voulons exécuter à l’intérieur du conteneur une fois qu’il est démarré. En spécifiant bash, nous disons à Docker de lancer un shell interactif (bash) dans le conteneur après son démarrage
