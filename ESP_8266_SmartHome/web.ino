void handleRoot() 
{
 String s = webpage;
 client.println(s);
}



void sensor_data() 
{
 String sensor_value = String(temp)+ ";"+ String(hum)+";"+ String(relTemp)+ ";" +String(relHum)+";"+String(senzorLum)+";"+String(relLum)+";"+String(gaz)+";"+String(waterLvl)+";"+String(window_poz);
 client.println(sensor_value);
}


void updateRef(String data) 
{
 //String act_state = server.arg("temp");
 Serial.println(data);
 Serial.println("POST");
 tempRef = extract(data, "?temp=", "&hum=");
 humRef = extract(data, "&hum=" , " HTTP/1.1");
 Serial.print("humRef is");

 Serial.println(humRef);



}
float extract(String data, String start_s, String stop_s){
String d="  ";
float dd=0;
int st = data.indexOf(start_s)+start_s.length();
int sp = data.indexOf(stop_s); 
for (int i = st; i<sp+1; i++ ){
  d[i-st] = data[i];
  Serial.println(i-st);   
}
dd = d.toFloat();
Serial.print("Recived:");
Serial.println(d);
Serial.println(dd);
Serial.println(st);
Serial.println(sp);
return dd;
}
  
