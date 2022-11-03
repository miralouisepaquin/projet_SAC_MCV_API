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

    this->on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.css", "text/css");
        });

    this->on("/SAC.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/SAC.png", "image/png");
        });

    this->on("/getNomEsp", HTTP_GET, [](AsyncWebServerRequest *request) {         
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askNomFour");          
        String lireNomDuFour =String(repString.c_str());         
        request->send(200, "text/plain", lireNomDuFour ); 
        }); 
    
    this->on("/getTemperatureMin", HTTP_GET, [](AsyncWebServerRequest *request) {         
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTemperatureMin");          
        String lireTemperatureMin =String(repString.c_str());         
        request->send(200, "text/plain", lireTemperatureMin ); 
        }); 

    this->on("/getBkColor", HTTP_GET, [](AsyncWebServerRequest *request) {         
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askBkColor"); 
        String lireBkColor =String(repString.c_str());         
        request->send(200, "text/plain", lireBkColor ); 
        });

    this->on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request) { 
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTemperature");
        String lireTemperature = String(repString.c_str());         
        request->send(200, "text/plain", lireTemperature );
    });

    this->on("/lireTemps", HTTP_GET, [](AsyncWebServerRequest *request) { 
        std::string repString = ""; 
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTemps");
        String lireTemps = String(repString.c_str());         
        request->send(200, "text/plain", lireTemps );
    });

    this->on("/ActionToDo", HTTP_POST, [](AsyncWebServerRequest *request) {         
        if (request->hasParam("actionToDo", true)) { 
            String actionToDo = request->getParam("actionToDo", true)->value(); 
 
            if (string(actionToDo.c_str()).compare(string("startAction")) == 0) { 
                if (ptrToCallBackFunction)(*ptrToCallBackFunction)("startAction"); 
                } 
            } 
        request->send(204); 
        });

    this->on("/getAllWoodOptions", HTTP_GET, [](AsyncWebServerRequest *request) { 
        Serial.println("getAllWoodOptions... ");
  
        //Your IP address or domain name with URL path
        HTTPClient http; 
        String apiRestAddress = "http://51.222.158.139:8080/api/bois";      
        http.begin(apiRestAddress); 
        http.GET(); 
        String response = http.getString();

        Serial.println(response);

        String tempToSend;
        StaticJsonDocument<5000> doc;
        deserializeJson(doc, response);
        JsonObject obj1 = doc.as<JsonObject>();
        std::string boisId;
        String  nomBois;
      
        for (JsonPair kv1 : obj1) {
            boisId = kv1.key().c_str();
            Serial.print("Element : ");Serial.println(boisId.c_str());

            JsonObject elem = obj1[boisId];
            nomBois = elem["nom"].as<String>();
            if(tempToSend!="") tempToSend += "&";
            tempToSend +=  String(boisId.c_str()) + String("&") + String(nomBois.c_str());
           
            Serial.print(nomBois);Serial.print(" ");
                          
            //Pour parcourir les éléments de l'objet
            //for (JsonPair kv2 : elem) {
            //    Serial.print("   Sous element : ");Serial.print(kv2.key().c_str());
            //    Serial.print("    :  ");Serial.println(kv2.value().as<char*>());
            //    }
            }
        
        request->send(200, "text/plain", tempToSend);
        }); 
   
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
        });
    
    this->begin();
};