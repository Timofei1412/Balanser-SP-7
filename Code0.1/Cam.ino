//640x480 pixels
// Serial.print(trackingCam.blob[i].area, DEC);
// Serial.print(trackingCam.blob[i].left, DEC);
// Serial.print(trackingCam.blob[i].right, DEC);
// Serial.print(trackingCam.blob[i].top, DEC);
// Serial.print(trackingCam.blob[i].bottom, DEC);

void cam(int info_type){
    int q = 5;
    uint8_t n = trackingCam.readBlobs(q); // read data about first 5 blobs
    posx = [];
    posy = [];
    for(int i = 0; i < n; i++){ // print information about all blobs
        if (trackingCam.blob[i].type == Block_blob_type){
        line_type = "brick";
        break;
        }else {
            posx[i] = trackingCam.blob[i].cx;// X coordinate of senter of the blob 
            posy[i] = trackingCam.blob[i].cy;// Y coordinate of senter of the blob
        }   
    }
    if (line_type != "brick"){ // Type of line choice
        if(posx[0] == posx[q]){
            line_type = "strait";
        }else if (posx[0] >= posx[q]){
            line_type = "left";
        }else if (posx[0] <= posx[q]){
            line_type = "right";
        }
    }
    if (info_type = 0){
        return line_type;
    }if (info_type = 1){
        return posx[q]-posx[0];
    }

    // // wait for the next frame
    // while(millis() - previousMillis < 33) 
    // {};
    // previousMillis = millis();
    
}