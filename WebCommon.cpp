#include "WebCommon.h"
#include <ESP8266WiFi.h>

String renderParameterRow(String paramName, String paramId, String paramValue, bool isReadonly, bool isPassword)
{
    String readonlyAttr = "readonly='readonly'";
    return String("<div class='input-group'><label class='input_label' for='" + paramId + "'>" + paramName + "</label><input type='" + (isPassword ? "password" : "text") + "' id='" + paramId + "' class='form-control' " + (isReadonly ? readonlyAttr : "") + " value='" + paramValue + "' /></div>");
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
