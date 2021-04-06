// Replaces placeholder with current button html
String processor(const String &var)
{
    //Serial.println(var);
    if (var == "BUTTONPLACEHOLDER")
    {
        String buttons = "";
        buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"0\"" + switchState(0) + "><span class=\"bbar\" name=\"^\"></span></label><br>\n";
        buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"1\"" + switchState(1) + "><span class=\"bbar\" name=\"<\"></span></label>\n";
        buttons += "<label class=\"block\"></label>\n";
        buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\"" + switchState(2) + "><span class=\"bbar\" name=\">\"></span></label><br>\n";
        buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"3\"" + switchState(3) + "><span class=\"bbar\" name=\"v\"></span></label>\n";
        buttons += "</div>\n<div class=\"control-2 control\">\n";
        buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\"><span class=\"bbar\" name=\"^\"></span></label>\n";
        buttons += "<p style=\"line-height: 34px; color: rgb(255, 255, 255);\">r Axis</p>\n";
        buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\"><span class=\"bbar\" name=\"v\"></span></label>";
        return buttons;
        
    }
    else if (var == "TEMPERATUREPLACEHOLDER"){
      return String(22);
    }
    return String();
}
//reutrn current pin state
String switchState(int switchPin)
{
    if (pinState[switchPin])
    {
        return "checked";
    }
    else
    {
        return "";
    }
}
