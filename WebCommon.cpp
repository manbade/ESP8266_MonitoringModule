#include "WebCommon.h"
#include <ESP8266WiFi.h>

String renderParameterRow(String paramName, String paramId, String paramValue, bool isReadonly, bool isPassword)
{
    String readonlyAttr = "readonly='readonly'";
    return String("<div class='input-group'><label class='input_label' for='" + paramId + "'>" + paramName + "</label><input type='" + (isPassword ? "password" : "text") + "' id='" + paramId + "' class='form-control' " + (isReadonly ? readonlyAttr : "") + " value='" + paramValue + "' /></div>");
}

String renderParameterList(String paramValue, String paramDesc)
{
    return String("<option value='" + paramValue + "'>" + paramDesc + "</option>");
}

String delay_list() {
   return String("<option value='60'>1 хвилина</option><option value='300'>5 хвилин</option><option value='600'>10 хвилин</option><option value='900'>15 хвилин</option><option value='1200'>20 хвилин</option><option value='1500'>25 хвилин</option><option value='1800'>30 хвилин</option><option value='2100'>35 хвилин</option><option value='2400'>40 хвилин</option><option value='2700'>45 хвилин</option><option value='3000'>50 хвилин</option><option value='3300'>55 хвилин</option><option value='3600'>1 година</option><option value='7200'>2 години</option><option value='10800'>3 години</option><option value='14400'>4 години</option><option value='18000'>5 годин</option><option value='21600'>6 годин</option>");
}

String renderTitle(String moduleName, String pageName)
{
    return String("<html lang='en'><head><title>" + moduleName + " - " + pageName + "</title><meta charset='utf-8'/><meta name='viewport' content='width=device-width, initial-scale=1'/>");
}

String renderAlert(String type, String text)
{
    return String("<div class='alert alert-" + type + "' role='alert'>" + text + "</div>");
}

String renderStyles(String styles)
{
    return String("<style>" + styles + "</style>");
}

String renderMenu(String delay)
{
    String text = FPSTR(mainMenu);
    text.replace("%d", delay);
    return text;
}
