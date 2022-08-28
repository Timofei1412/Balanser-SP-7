void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}

// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data) {
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}
// Counter
long long cpt = 0;
void callback() {
  intFlag = true;
  digitalWrite(13, digitalRead(13) ^ 1);
}

void Gyro_out(int info) {
  while (!intFlag);
  intFlag = false;
  // Read accelerometer and gyroscope
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);

  // Create 16 bits values from 8 bits data
  // Accelerometer
  int16_t ax = -(Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = (Buf[4] << 8 | Buf[5]);
  // Gyroscope
  int16_t gx = -(Buf[8] << 8 | Buf[9]);
  int16_t gy = -(Buf[10] << 8 | Buf[11]);
  int16_t gz = (Buf[12] << 8 | Buf[13]);
  
  //Type of output choise
  return info== 1?gz:az;
  //OR:
  //  if (info == 1) {
  //    return gz; //Gyro z angle(Code0.1 line 82)
  //   }
  //  if (info == 2) {
  //    return az;// Aссel z angle(Code0.1 line 83)
  //   }
  
}
