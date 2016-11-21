#ifndef WEBCOMMON_H
#define WEBCOMMON_H

#include <Arduino.h>
#include "JsonConfig.h"

const char stylesInclude[] PROGMEM = "<link rel='stylesheet' type='text/css' href='/styles.css' />";

const char mainMenu[] PROGMEM = "<div class='header'><ul>\
<li><a href='/'>Головна</a></li>\
<li><a href='/wifiSetup'>Налаштування WiFi</a></li>\
<li><a href='/cloudSetup'>Хмарні сховища</a></li>\
<li><a href='/sensors'>Давачі даних</a></li>\
<li><a class='/reboot' href='/reboot?reboot_delay=%d'>Перезавантаження</a></li>\
</ul></div>";

const char styles[] PROGMEM = "body{font-family: tahoma;font-size: 14px}.header{margin: 10px 0}.header ul{margin: 0;padding: 0}.header li{margin: 0 5px 10px 0;padding: 0;list-style: none;display: inline-block;*display: inline;}.header a{padding: 3px 12px;text-decoration: none;color: #999;line-height: 100%}.header a:hover{color: #000}.header .current a{background: #999;color: #fff;border-radius: 3px}.header.right ul{text-align: right}.header.center ul{text-align: center}.reboot{color: red !important}.container{border: 1px solid #ddd;border-radius: 4px;padding: 5px;margin-top: 5px}#main_block{text-align: center;width: 100%}.header, .container{display: inline-block;vertical-align: middle}a{text-decoration: none}p{margin: 5px}hr{border: 0;border-top: 1px solid #eee}h2{display: inline-block;margin: 10px 0px}.input-group{display: inline-block;position: relative;border-collapse: separate;margin: 0 auto;width: 100%;padding: 5px}.input_label{display: inline-block;float: left;display: inline;width: 35%}.form-control{position: relative;width: 41%;display: inline;height: 34px;padding: 6px 12px;font-size: 14px;line-height: 1.42857143;color: #555;background-color: #fff;background-image: none;border: 1px solid #ccc;border-radius: 4px;-webkit-box-shadow: inset 0 1px 1px rgba(0, 0, 0, .075);box-shadow: inset 0 1px 1px rgba(0, 0, 0, .075);-webkit-transition: border-color ease-in-out .15s, -webkit-box-shadow ease-in-out .15s;-o-transition: border-color ease-in-out .15s, box-shadow ease-in-out .15s;transition: border-color ease-in-out .15s, box-shadow ease-in-out .15s}@media screen and (max-width: 1600px){#main_block{text-align: center;width: 31%;margin: 0 auto}.form-control{width: 60%;text-align: center}\
.input_label{width: 35%}}@media screen and (max-width: 1200px){#main_block{text-align: center;width: 50%;margin: 0 auto}.header{width: 85%}}@media screen and (max-width: 768px){#main_block{text-align: center;width: 100%;margin: 0 auto}}.form-control:focus{border-color: #66afe9;outline: 0;-webkit-box-shadow: inset 0 1px 1px rgba(0,0,0,.075), 0 0 8px rgba(102, 175, 233, .6);box-shadow: inset 0 1px 1px rgba(0,0,0,.075), 0 0 8px rgba(102, 175, 233, .6)}\
.marginTop0{margin-top: 0 !important}.btn{display: inline-block;padding: 6px 12px;font-size: 14px;font-weight: 400;line-height: 1.42857143;text-align: center;white-space: nowrap;vertical-align: middle;-ms-touch-action: manipulation;touch-action: manipulation;cursor: pointer;-webkit-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;background-image: none;border: 1px solid transparent;border-radius: 4px}.btn-default{color: #333;background-color: #fff;border-color: #ccc}.btn-default{text-shadow: 0 -1px 0 rgba(0,0,0,.2);-webkit-box-shadow: inset 0 1px 0 rgba(255,255,255,.15),0 1px 1px rgba(0,0,0,.075);box-shadow: inset 0 1px 0 rgba(255,255,255,.15),0 1px 1px rgba(0,0,0,.075)}.btn-default{text-shadow: 0 1px 0 #fff;background-image: -webkit-linear-gradient(top,#fff 0,#e0e0e0 100%);background-image: -o-linear-gradient(top,#fff 0,#e0e0e0 100%);background-image: -webkit-gradient(linear,left top,left bottom,from(#fff),to(#e0e0e0));background-image: linear-gradient(to bottom,#fff 0,#e0e0e0 100%);filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffffff', endColorstr='#ffe0e0e0', GradientType=0);background-repeat: repeat-x;border-color: #dbdbdb}.alert{padding: 15px;margin-bottom: 20px;margin-top: 20px;border: 1px solid transparent;border-radius: 4px}.alert-danger{color: #a94442;background-color: #f2dede;border-color: #ebccd1}\
.alert-info{color: #31708f;background-color: #d9edf7;border-color: #bce8f1}.alert{text-shadow: 0 1px 0 rgba(255,255,255,.2);-webkit-box-shadow: inset 0 1px 0 rgba(255,255,255,.25),0 1px 2px rgba(0,0,0,.05);box-shadow: inset 0 1px 0 rgba(255,255,255,.25),0 1px 2px rgba(0,0,0,.05)}.alert-danger{background-image: -webkit-linear-gradient(top,#f2dede 0,#e7c3c3 100%);background-image: -o-linear-gradient(top,#f2dede 0,#e7c3c3 100%);background-image: -webkit-gradient(linear,left top,left bottom,from(#f2dede),to(#e7c3c3));background-image: linear-gradient(to bottom,#f2dede 0,#e7c3c3 100%);filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#fff2dede', endColorstr='#ffe7c3c3', GradientType=0);background-repeat: repeat-x;border-color: #dca7a7}.alert-info{background-image: -webkit-linear-gradient(top,#d9edf7 0,#b9def0 100%);background-image: -o-linear-gradient(top,#d9edf7 0,#b9def0 100%);background-image: -webkit-gradient(linear,left top,left bottom,from(#d9edf7),to(#b9def0));background-image: linear-gradient(to bottom,#d9edf7 0,#b9def0 100%);filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffd9edf7', endColorstr='#ffb9def0', GradientType=0);background-repeat: repeat-x;border-color: #9acfea}\
button[data-balloon]{overflow:visible}[data-balloon]{position:relative}[data-balloon]:after,[data-balloon]:before{-ms-filter:'progid:DXImageTransform.Microsoft.Alpha(Opacity=0)';filter:alpha(opacity=0);-khtml-opacity:0;-moz-opacity:0;opacity:0;pointer-events:none;-webkit-transition:all .18s ease-out .18s;transition:all .18s ease-out .18s;bottom:100%;left:50%;position:absolute;z-index:10;-webkit-transform:translate(-50%,10px);-ms-transform:translate(-50%,10px);transform:translate(-50%,10px);-webkit-transform-origin:top;-ms-transform-origin:top;transform-origin:top}[data-balloon]:after{background:rgba(17,17,17,.9);border-radius:4px;color:#fff;content:attr(data-balloon);font-size:12px;padding:.5em 1em;white-space:nowrap;margin-bottom:11px}[data-balloon]:before{background: white;background-size:100% auto;height:6px;width:18px;content:'';margin-bottom:5px}[data-balloon]:hover:after,[data-balloon]:hover:before,[data-balloon][data-balloon-visible]:after,[data-balloon][data-balloon-visible]:before{-ms-filter:'progid:DXImageTransform.Microsoft.Alpha(Opacity=100)';filter:alpha(opacity=100);-khtml-opacity:1;-moz-opacity:1;opacity:1;pointer-events:auto;-webkit-transform:translate(-50%,0);-ms-transform:translate(-50%,0);transform:translate(-50%,0)}[data-balloon][data-balloon-break]:after{white-space:normal}[data-balloon-pos=down]:after,[data-balloon-pos=down]:before{bottom:auto;left:50%;top:100%;-webkit-transform:translate(-50%,-10px);-ms-transform:translate(-50%,-10px);transform:translate(-50%,-10px)}[data-balloon-pos=down]:after{margin-top:11px}[data-balloon-pos=down]:before{background: white;background-size:100% auto;height:6px;width:18px;margin-top:5px;margin-bottom:0}[data-balloon-pos=down]:hover:after,\
[data-balloon-pos=down]:hover:before,[data-balloon-pos=down][data-balloon-visible]:after,[data-balloon-pos=down][data-balloon-visible]:before{-webkit-transform:translate(-50%,0);-ms-transform:translate(-50%,0);transform:translate(-50%,0)}[data-balloon-pos=left]:after,[data-balloon-pos=left]:before{bottom:auto;left:auto;right:100%;top:50%;-webkit-transform:translate(10px,-50%);-ms-transform:translate(10px,-50%);transform:translate(10px,-50%)}[data-balloon-pos=left]:after{margin-right:11px}[data-balloon-pos=left]:before{background: white no-repeat;background-size:100% auto;height:18px;width:6px;margin-right:5px;margin-bottom:0}[data-balloon-pos=left]:hover:after,[data-balloon-pos=left]:hover:before,[data-balloon-pos=left][data-balloon-visible]:after,[data-balloon-pos=left][data-balloon-visible]:before{-webkit-transform:translate(0,-50%);-ms-transform:translate(0,-50%);transform:translate(0,-50%)}[data-balloon-pos=right]:after,[data-balloon-pos=right]:before{bottom:auto;left:100%;top:50%;-webkit-transform:translate(-10px,-50%);-ms-transform:translate(-10px,-50%);transform:translate(-10px,-50%)}[data-balloon-pos=right]:after{margin-left:11px}[data-balloon-pos=right]:before{background: white;background-size:100% auto;height:18px;width:6px;margin-bottom:0;margin-left:5px}[data-balloon-pos=right]:hover:after,[data-balloon-pos=right]:hover:before,[data-balloon-pos=right][data-balloon-visible]:after,[data-balloon-pos=right][data-balloon-visible]:before{-webkit-transform:translate(0,-50%);-ms-transform:translate(0,-50%);transform:translate(0,-50%)}[data-balloon-length]:after{white-space:normal}[data-balloon-length=small]:after{width:80px}[data-balloon-length=medium]:after{width:150px}[data-balloon-length=large]:after{width:260px}[data-balloon-length=xlarge]:after{width:90vw}@media screen and (min-width:768px){[data-balloon-length=xlarge]:after{width:380px}}[data-balloon-length=fit]:after{width:100%}\
";
const char rebootScripts[] PROGMEM =
"<script type='text/javascript'>\
function \
getUrlVars()\
{\
var vars = [], hash;\
var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');\
for(var i = 0; i < hashes.length; i++)\
{\
hash = hashes[i].split('=');\
vars.push(hash[0]);\
vars[hash[0]] = hash[1];\
}\
return vars;\
}\
//window.onload = function() {\
//var params = getUrlVars();\
//var delay = parseInt(params['reboot_delay'], 10);\
//console.log(delay);\
//var txt = document.getElementById('info');\
//var counter = delay;\
//var id = setInterval(function () {\
//txt.innerHTML = 'Перезавантаження через ' + counter + ' секунд.';\
//counter--;\
//console.log(counter);\
//if (counter < 0) {\
//clearInterval(id);\
//txt.innerHTML = 'Module was rebooted.';\
//console.log('Reboot!');\
//}\
//}, 1000);\
//}\
</script>";

const char scripts[] PROGMEM =
"<script type='text/javascript'>\
function set_val(elmnt){if(elmnt.checked){elmnt.setAttribute('value','1');}else{elmnt.setAttribute('value','0');}};function tooltip(selector,datalength,text,textpos){var element=document.querySelector(selector);element.setAttribute('data-balloon-length',datalength);element.setAttribute('data-balloon',text);element.setAttribute('data-balloon-pos',textpos);};function toogle_set(selector){var elements=document.querySelectorAll(selector);for(var i=0;i<elements.length;i++){elements[i].setAttribute('type','checkbox');elements[i].setAttribute('onclick','set_val(this)');var element_val=elements[i].value;if(element_val==1){var check=elements[i].checked=true;}else{elements[i].checked=false;}}};function rssi_color(){var elements=document.getElementById('sta_ssid');for(var i=0;i<elements.childElementCount;i++){var dbm=elements.options[i].getAttribute('data-dbm');if(dbm>-100&&dbm<-65){elements.options[i].style.color='red';}else{elements.options[i].style.color='green';elements.options[i].style.fontWeight='bold';}}};window.onload=function(){if(window.location.href.indexOf('wifiSetup')>-1){toogle_set('input[id*=toogle]');rssi_color();var e_ssid=document.getElementById('sta_ssid'),ssid_name_tag=document.getElementById('ssid_name'),ssid_tag_val=ssid_name_tag.innerText;if(e_ssid){for(var i=0;i<e_ssid.length;i++){if(e_ssid.options[i].value===ssid_tag_val){e_ssid.options[i].selected=true;}}}};if(window.location.href.indexOf('cloudSetup')>-1){toogle_set('input[id*=toogle]');tooltip('input[id=narodmon_toogle]','medium','НароднийМоніторинг вимагає мін.інтервалу експорту в 5 хвилин (300 секунд)','right');tooltip('input[id=deepsleep_toogle]','medium','Дозволяє переводити систему в період між передачею даних в режим мінімального енергоспоживання(20мкА)','right');var e_get_d=document.getElementById('get_data_delay'),name_tag=document.getElementById('data_delay').innerText;if(e_get_d){for(var i=0;i<e_get_d.length;i++){if(e_get_d.options[i].value==name_tag){e_get_d.options[i].selected=true;}}}};if(window.location.href.indexOf('sensors')>-1){toogle_set('input[id*=sensor]');tooltip('input[id=sensor_bmp180_on]','medium','SDL - GPIO2(D3), SDA - GPIO14(D4)','right');tooltip('input[id=sensor_dht_on]','small','GPIO4 (D2)','right');tooltip('input[id=sensor_ds18b20_on]','small','GPIO0 (D3)','right');tooltip('input[id=sensor_analog_on]','medium','ADC Pin (only in ESP-07 and older)','right');};};function saveFormData(pageToRecall){var data=[];var inputs=document.getElementsByTagName('input');for(var i=0;i<inputs.length;i++){var input=inputs[i];if(input.type=='text'||input.type=='password'||input.type=='checkbox'){data.push({key:input.id,value:input.value});}} var e_ssid=document.getElementById('sta_ssid');if(e_ssid){var selected_opt=e_ssid.options[e_ssid.selectedIndex].value;data.push({key:'sta_ssid',value:selected_opt});} var e_delay=document.getElementById('get_data_delay');if(e_delay){var selected_delay=e_delay.options[e_delay.selectedIndex].value;data.push({key:'get_data_delay',value:selected_delay});} var url=pageToRecall+'?';for(var j=0;j<data.length;j++){var param=data[j];url+=param.key+'='+param.value+'&';} document.location=url;}\
</script>";

const char headEnd[] PROGMEM = "</head>";
const char bodyStart[] PROGMEM = "<body><div id='main_block'>";
const char bodyEnd[] PROGMEM = "</div></body></html>";

String renderParameterRow(String paramName, String paramId, String paramValue, bool isReadonly = false, bool isPassword = false);
String renderParameterList(String paramValue,  String paramDesc, String optioanAttr);
String delay_list();
String renderTitle(String pageName, String moduleName);
String renderAlert(String type, String text);
String renderStyles(String styles);
String renderSsid(WiFiData data);
String renderMenu();

#endif
