/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
using namespace std;

//variable pour le id du bois
String id = "";

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
    }

void MyServer::getTemp(float temperature) {
  
}

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
        }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
        });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
        });

    //Route du script index.css
    this->on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.css", "text/css");
        });

    //Route pour l'image logo SAC
    this->on("/SAC.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/SAC.png", "image/png");
        });

    //Route pour avoir le BkGround color pour la lumière
    this->on("/getBkColor", HTTP_GET, [](AsyncWebServerRequest *request) {         
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askBkColor"); 
        String lireBkColor =String(repString.c_str());         
        request->send(200, "text/plain", lireBkColor ); 
        });

    //Route pour lire la température
    this->on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request) { 
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTemperature");
        String lireTemperature = String(repString.c_str());         
        request->send(200, "text/plain", lireTemperature );
    });

    //Route pour lire le temps de séchage actuel
    this->on("/lireTemps", HTTP_GET, [](AsyncWebServerRequest *request) { 
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTemps");
        String lireTemps = String(repString.c_str());         
        request->send(200, "text/plain", lireTemps );
    });

    //Route pour envoyer une action (démarrer dans ce cas)
    this->on("/ActionToDo", HTTP_POST, [](AsyncWebServerRequest *request) {         
        if (request->hasParam("actionToDo", true)) { 
            String actionToDo = request->getParam("actionToDo", true)->value(); 
 
            if (string(actionToDo.c_str()).compare(string("startAction")) == 0) { 
                if (ptrToCallBackFunction)(*ptrToCallBackFunction)("startAction"); 
                }
        }
        request->send(204); 
        });

    //Route pour call API pour tous les noms de bois.
    this->on("/getAllWood", HTTP_GET, [](AsyncWebServerRequest *request) {   
        //Your IP address or domain name with URL path
        HTTPClient http; 
        String apiRestAddress = "http://51.222.158.139:8080/api/bois";    
        http.begin(apiRestAddress); 
        http.GET(); 
        String response = http.getString();
        //Permet de valider si [] au extrémité et les enlever
        if(response[0]=='[')
            response[0] =' ';
        if(response[response.length()-1]==']')
            response[response.length()-1] =' ';

        String tempToSend;
        StaticJsonDocument<5000> doc;
        deserializeJson(doc, response);
        JsonObject obj1 = doc.as<JsonObject>();
        std::string wood;
        String  woodName;
        String  woodId;
      
        for (JsonPair kv1 : obj1) {
            wood = kv1.key().c_str();
            if(wood.compare("nom") == 0){
                woodName = kv1.value().as<String>().c_str();
                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(woodName.c_str());
                }
            if(wood.compare("id") == 0){
                woodId = kv1.value().as<String>().c_str();
                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(woodId.c_str());
                }      
        }

        Serial.println("TempToSend : " + tempToSend);

        request->send(200, "text/plain", tempToSend);
    });

    //Route pour call API pour les options du bois choisit.
    this->on("/getWoodOption", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("id", true)) { 
            id = request->getParam("id", true)->value();
        }
  
        //Your IP address or domain name with URL path
        HTTPClient http; 
        String apiRestAddress = "http://51.222.158.139:8080/api/bois/"+id;    
        http.begin(apiRestAddress); 
        http.GET(); 
        String response = http.getString();
        //Permet de valider si [] au extrémité et les enlever
        if(response[0]=='[')
            response[0] =' ';
        if(response[response.length()-1]==']')
            response[response.length()-1] =' ';

        String tempToSend;
        String stringToSend;
        StaticJsonDocument<10000> doc;
        deserializeJson(doc, response);
        JsonObject obj1 = doc.as<JsonObject>();
        std::string wood;
        String  woodName;
        String  woodType;
        String  woodOrigine;
        String  woodSechage;
        String  woodTemperature;
        
      
        for (JsonPair kv1 : obj1) {
            wood = kv1.key().c_str();
            if(wood == "nom"){
                woodName = kv1.value().as<String>();

                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(wood.c_str()) + String("&") + String(woodName.c_str());
            }   
            if(wood == "type"){
                woodType = kv1.value().as<String>();

                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(wood.c_str()) + String("&") + String(woodType.c_str());
            }  
            if(wood == "origine"){
                woodOrigine = kv1.value().as<String>();

                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(wood.c_str()) + String("&") + String(woodOrigine.c_str());
            } 
            if(wood == "tempsSechage"){
                woodSechage = kv1.value().as<String>();

                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(wood.c_str()) + String("&") + String(woodSechage.c_str());
            }
            if(wood == "temperature_min"){
                woodTemperature = kv1.value().as<String>();

                if(tempToSend!="") tempToSend += "&";            
                tempToSend += String(wood.c_str()) + String("&") + String(woodTemperature.c_str());
            }                            
        }

        stringToSend += String("woodOption")+String(" ")+String(woodName.c_str());
        stringToSend += String(" ") + String(woodType.c_str());
        stringToSend += String(" ") + String(woodOrigine.c_str());
        stringToSend += String(" ") + String(woodSechage.c_str());
        stringToSend += String(" ") + String(woodTemperature.c_str());

        if (ptrToCallBackFunction)(*ptrToCallBackFunction)(stringToSend.c_str());

        request->send(200, "text/plain", tempToSend);
    });
   
    //Route au cas ou la route demandée n'existe pas
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
        });
    
    this->begin();
};