function getFromESP_getNom(){ 
    var xhttp = new XMLHttpRequest();     
    xhttp.onreadystatechange = function () { 
        if (this.readyState == 4 && this.status == 200) { 
            document.getElementById("nom").innerHTML = this.responseText; 
        } 
    }; 
    xhttp.open("GET", "getNomEsp", true); 
    xhttp.send(); 
} 

function getFromESP_getTemperatureMin(){ 
    var xhttp = new XMLHttpRequest();     
    xhttp.onreadystatechange = function () { 
        if (this.readyState == 4 && this.status == 200) { 
            document.getElementById("temperature_min").innerHTML = this.responseText; 
        } 
    }; 
    xhttp.open("GET", "getTemperatureMin", true); 
    xhttp.send(); 
}

setInterval(function getBackGroundColor(){
    var xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
    if(this.status == 200){
        document.getElementById("color").style.backgroundColor = this.responseText;}
    }; 
    xhttp.open("GET", "getBkColor", true);
    xhttp.send();
}, 1000);

setInterval(function getTemperature(){
    var xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
    if(this.status == 200){
        document.getElementById("temp").innerHTML = this.responseText;}
    }; 
    xhttp.open("GET", "lireTemperature", true);
    xhttp.send();
}, 1000);

setInterval(function getTempsActuel(){
    var xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
    if(this.status == 200){
        document.getElementById("temps_actuel").innerHTML = this.responseText;}
    }; 
    xhttp.open("GET", "lireTemps", true);
    xhttp.send();
}, 1000);

function doAction(actionToDo){ 
    var params = String("actionToDo") + String("=") + String(actionToDo);     
    var xhttp = new XMLHttpRequest();     
    xhttp.open("POST", "ActionToDo", true); 
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');     
    xhttp.send(params); 
} 

function getFromESP_getAllWoods(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState == 4 && this.status == 200){
            var arrayOfStrings = this.responseText.split("&");
            for (i = 0; i < arrayOfStrings.length; i=i+2){
                var x = document.getElementById("typeBois_ListBox_Select");
                var option = document.createElement("option");
                option.value = arrayOfStrings[i];
                option.text = arrayOfStrings[i+1];
                x.add(option);
                } 

            //Refresh le contenu
            var siteHeader = document.getElementById('typeBois_ListBox_Select');
            siteHeader.style.display='none';
            siteHeader.offsetHeight; // no need to store this anywhere, the reference is enough
            siteHeader.style.display='block';

            }
    };
    xhttp.open("GET", "getAllWoodOptions", true);
    xhttp.send();
} 


