#pragma once

void RunUplinkExceptionHandling();
void hSignalSIGSEGV(int signum);
void hSignalSIGFPE(int signum);
void hSignalSIGPIPE(int signum);
