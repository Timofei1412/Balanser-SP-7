int PID(int set, int inp, int kp, int ki, int kd, int dt) {
  int P = set - inp;
  int I = I + (set - inp) * dt;
  int D = (P - prevErr) / dt;
  int prevErr = P;
  return p * kp + I * ki + D * kd;
}
