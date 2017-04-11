var xmlHttp = createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp = new XMLHttpRequest();
 }else{
  xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}
function load(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/configs.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
  jsonResponse=JSON.parse(xmlHttp.responseText);
  loadBlock();
  }
 }
}
function loadSET(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/settings.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
  jsonResponse=JSON.parse(xmlHttp.responseText);
  loadBlock();
  }
 }
}
function loadSTAT(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/state.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
  jsonResponse=JSON.parse(xmlHttp.responseText);
  loadBlock();
  }
 }
 }
function loadDATA(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/data.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
  jsonResponse=JSON.parse(xmlHttp.responseText);
  loadBlock();
  }
 }
}
function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
 handleServerResponse();
}


function refreshDATA(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/state.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
  jsonResponse=JSON.parse(xmlHttp.responseText);
  loadRefreshBlock(jsonResponse);
  }
 }
    setTimeout('refreshDATA()',5000);
}

function loadRefreshBlock(data2) {
  document.getElementById('date').value = data2['Year']+' / '+data2['Month']+' / '+data2['Day'];  
  document.getElementById('time').value = data2['Hour']+' : '+data2['Minute'];  
  delete data2['Year'];  delete data2['Month'];  delete data2['Day'];  delete data2['Hour'];  delete data2['Minute'];
    
  for (var key in data2) {
    if (document.getElementById(key).value!= data2[key]){
      document.getElementById(key).value = data2[key];
    }
  }
  if (data2['ManualMod']=="авто. управление")  loadbatonState(false);
  else loadbatonState(true);    

}

function loadbatonState(data2){
    var elements = document.getElementsByClassName('disabled');
    for (var key= 0; key < elements.length;key++) {
       if (elements[key].disabled != data2) elements[key].disabled = data2;    
  }    
}

function val(id){
 var v = document.getElementById(id).value;
 return v;
}
function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}
function save_status(submit,request){
 //old_submit = submit.value;
 request.onreadystatechange = function() {
 if (request.readyState != 4 && request.status == 200) return;  
  submit.value = request.responseText;
  setTimeout(function(){
 //  submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 // submit.value = 'Подождите...';
 submit_disabled(true);
}
function submit_disabled(request){
 var inputs = document.getElementsByTagName('input'); 
 for (var i = 0; i < inputs.length; i++) {
   if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}

function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
