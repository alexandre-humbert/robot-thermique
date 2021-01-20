var value;
var mode=0;
var tmin=20;
var tmax=40;
var fps=1;
var disp_temp =1;
var temp_auto =true;

document.getElementById("etat_on").style.display = "none";
document.getElementById("settings").style.display = "none";

function start(){
value = setInterval(refresh, 1000);
document.getElementById("etat_off").style.display = "none";
document.getElementById("etat_on").style.display = "block";
document.getElementById("svg").style.display = "block";


}


function stop(){
clearInterval(value);
document.getElementById("etat_off").style.display = "block";
document.getElementById("etat_on").style.display = "none";
document.getElementById("svg").style.display = "none";
}

function refresh(){
if (temp_auto){
$.get("/cgi-bin/interface.cgi",function(data){
document.getElementById("svg").innerHTML = data;
if (disp_temp){
afficher_temp();
}
});
}
else {
$.get("/cgi-bin/interface.cgi?"+tmin.toString()+"&"+tmax.toString()+"",function(data){
document.getElementById("svg").innerHTML = data;
if (disp_temp){
afficher_temp();
}
});
}
}


function avancer(){$.get("/cgi-bin/interface.cgi?20&20&av");}
function stopper(){$.get("/cgi-bin/interface.cgi?&&st");}
function reculer(){$.get("/cgi-bin/interface.cgi?&&ar");}
function avant_droite(){$.get("/cgi-bin/interface.cgi?&&dr");}
function avant_gauche(){$.get("/cgi-bin/interface.cgi?&&ga");}
function droite(){$.get("/cgi-bin/interface.cgi?&&td");}
function gauche(){$.get("/cgi-bin/interface.cgi?&&tg");}
function arr_gauche(){$.get("/cgi-bin/interface.cgi?&&ag");}
function arr_droite(){$.get("/cgi-bin/interface.cgi?&&ad");}

function switch_mode(){
if (mode==0){
document.getElementById("settings").style.display = "block";
document.getElementById("btn_set").innerHTML = "Mode classique";
document.getElementById("tmin").value=tmin;
document.getElementById("tmax").value=tmax;
document.getElementById("fps").value=fps;
document.getElementById("temp_auto").checked = temp_auto;
update_values();
mode=1;
}
else{
document.getElementById("settings").style.display = "none";
document.getElementById("btn_set").innerHTML = "Mode avancé";
mode=0;
}
}

function update_values(){
tmin=document.getElementById("tmin").value;
tmax=document.getElementById("tmax").value;
fps=document.getElementById("fps").value;


document.getElementById("value_tmin").innerHTML=tmin;
document.getElementById("value_tmax").innerHTML=tmax;
document.getElementById("value_fps").innerHTML=fps;
valider();
}

function switch_temp(){
disp_temp= !disp_temp;
}

function afficher_temp(){
var collection;
collection = document.getElementsByClassName("temp");
for (var item of collection){
	if(item.style.display!="none"){
		item.style.display="none";
		}
	else{
		item.style.display="block";}
}
}



function valider(){
clearInterval(value);
value = setInterval(refresh, Math.floor(1000/fps));
temp_auto = document.getElementById("temp_auto").checked;

}

function envoyer_cmd(){
var inputVal = document.getElementById("cmd").value;
inputVal = inputVal.replace(" ","+");
$.get("/cgi-bin/interface.cgi?20&20&"+inputVal);
}


