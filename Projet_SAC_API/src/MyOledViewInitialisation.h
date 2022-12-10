/**
    Class MyOledViewInitialisation : Initialisation des valeur pour le OLed
    @file MyOledViewInitialisation.h 
    @author Mira Paquin
    @version 1.1 01/12/2022 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      01/12/2022   MLP         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        //Définition 
            #include "MyOledViewInitialisation.h"
            class MyOledViewInitialisation: public MyOledView {
                public:
                
                private:
                    //Mettre la vue dans cette méthode
                    void display( Adafruit_SSD1306 *adafruit);
        ...
        //Utilisation
        myOledViewInitialisation = new MyOledViewInitialisation();
        myOled->displayView(myOledViewInitialisation);
**/

#define MYOLEDVIEWINITIALISATION_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"
#include <string>

class MyOledViewInitialisation: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);
        void setIdDuSysteme(std::string val);
        void setSensibiliteBoutonAction(std::string val);
        void setSensibiliteBoutonReset(std::string val);

    private:
        std::string nomDuSysteme;
        std::string idDuSysteme;
        std::string SensibiliteBoutonAction;
        std::string SensibiliteBoutonReset;   
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);         
};