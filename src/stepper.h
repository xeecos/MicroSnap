// #pragma once


// void begin() 
// {
// 	GCONF_register.i_scale_analog = 1;
// 	GCONF_register.internal_rsense = 0; // OTP
// 	GCONF_register.en_spreadcycle = 0; // OTP
// 	GCONF_register.multistep_filt = 1; // OTP
//     GCONF_register.pdn_disable = 1;
// 	GCONF_register.mstep_reg_select = 1;
// 	IHOLD_IRUN_register.iholddelay = 1; // OTP
// 	TPOWERDOWN_register.sr = 20;
// 	CHOPCONF_register.sr = 0x10000053;
// 	PWMCONF_register.sr = 0xC10D0024;
// }
// static float Rsense = 0.1;
// void rms_current(uint16_t mA) {
//   uint8_t CS = 32.0*1.41421*mA/1000.0*(Rsense+0.02)/0.325 - 1;
//   // If Current Scale is too low, turn on high sensitivity R_sense and calculate again
//   if (CS < 16) {
//     vsense(true);
//     CS = 32.0*1.41421*mA/1000.0*(Rsense+0.02)/0.180 - 1;
//   } else { // If CS >= 16, turn off high_sense_r
//     vsense(false);
//   }

//   if (CS > 31)
//     CS = 31;

//   irun(CS);
//   ihold(CS*holdMultiplier);
//   //val_mA = mA;
// }

// void microsteps(uint16_t ms) {
//   switch(ms) {
//     case 256: mres(0); break;
//     case 128: mres(1); break;
//     case  64: mres(2); break;
//     case  32: mres(3); break;
//     case  16: mres(4); break;
//     case   8: mres(5); break;
//     case   4: mres(6); break;
//     case   2: mres(7); break;
//     case   0: mres(8); break;
//     default: break;
//   }
// }
// void push() {
// 	GCONF(GCONF_register.sr);
// 	IHOLD_IRUN(IHOLD_IRUN_register.sr);
// 	SLAVECONF(SLAVECONF_register.sr);
// 	TPOWERDOWN(TPOWERDOWN_register.sr);
// 	TPWMTHRS(TPWMTHRS_register.sr);
// 	VACTUAL(VACTUAL_register.sr);
// 	CHOPCONF(CHOPCONF_register.sr);
// 	PWMCONF(PWMCONF_register.sr);
// }

// uint8_t calcCRC(uint8_t datagram[], uint8_t len) {
// 	uint8_t crc = 0;
// 	for (uint8_t i = 0; i < len; i++) {
// 		uint8_t currentByte = datagram[i];
// 		for (uint8_t j = 0; j < 8; j++) {
// 			if ((crc >> 7) ^ (currentByte & 0x01)) {
// 				crc = (crc << 1) ^ 0x07;
// 			} else {
// 				crc = (crc << 1);
// 			}
// 			crc &= 0xff;
// 			currentByte = currentByte >> 1;
// 		}
// 	}
// 	return crc;
// }

// uint8_t serial_available() {
// 	int out = 0;
// 	return out;
// }
// uint8_t serial_read() {
// 	int out = 0;
// 	return out;
// }
// uint8_t serial_write(const uint8_t data) {
// 	int out = 0;
// 	uint8_t c = data;
// 	uart_send(&c,1);
// 	return out;
// }

// static constexpr uint8_t TMC_READ = 0x00, TMC_WRITE = 0x80;
// static constexpr uint8_t  TMC2208_SYNC = 0x05, TMC2208_SLAVE_ADDR = 0x00;
// static constexpr uint8_t abort_window = 5;
// static constexpr uint8_t max_retries = 2;
// void write(uint8_t addr, uint32_t regVal) {
// 	uint8_t len = 7;
// 	addr |= TMC_WRITE;
// 	uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, (uint8_t)(regVal>>24), (uint8_t)(regVal>>16), (uint8_t)(regVal>>8), (uint8_t)(regVal>>0), 0x00};

// 	datagram[len] = calcCRC(datagram, len);

// 	for(uint8_t i=0; i<=len; i++) {
// 		bytesWritten += serial_write(datagram[i]);
// 	}

// 	DelayMs(2);
// }
// uint32_t read(uint8_t addr) {
// 	constexpr uint8_t len = 3;
// 	addr |= TMC_READ;
// 	uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, 0x00};
// 	datagram[len] = calcCRC(datagram, len);
// 	uint64_t out = 0x00000000UL;

// 	for (uint8_t i = 0; i < max_retries; i++) {
// 		out = _sendDatagram(datagram, len, abort_window);

// 		DelayMs(2);

// 		CRCerror = false;
// 		uint8_t out_datagram[] = {
// 			static_cast<uint8_t>(out>>56),
// 			static_cast<uint8_t>(out>>48),
// 			static_cast<uint8_t>(out>>40),
// 			static_cast<uint8_t>(out>>32),
// 			static_cast<uint8_t>(out>>24),
// 			static_cast<uint8_t>(out>>16),
// 			static_cast<uint8_t>(out>> 8),
// 			static_cast<uint8_t>(out>> 0)
// 		};
// 		uint8_t crc = calcCRC(out_datagram, 7);
// 		if ((crc != static_cast<uint8_t>(out)) || crc == 0 ) {
// 			CRCerror = true;
// 			out = 0;
// 		} else {
// 			break;
// 		}
// 	}

// 	return out>>8;
// }

// uint64_t _sendDatagram(uint8_t datagram[], const uint8_t len, uint16_t timeout) {
// 	while (serial_available() > 0) serial_read(); // Flush

// 	for(int i=0; i<=len; i++) serial_write(datagram[i]);

// 	DelayMs(this->replyDelay);

// 	// scan for the rx frame and read it
// 	uint32_t ms = SYS_GetSysTickCnt();
// 	uint32_t sync_target = (static_cast<uint32_t>(datagram[0])<<16) | 0xFF00 | datagram[2];
// 	uint32_t sync = 0;

// 	do {
// 		uint32_t ms2 = SYS_GetSysTickCnt();
// 		if (ms2 != ms) {
// 			// 1ms tick
// 			ms = ms2;
// 			timeout--;
// 		}
// 		if (!timeout) return 0;

// 		int16_t res = serial_read();
// 		if (res < 0) continue;

// 		sync <<= 8;
// 		sync |= res & 0xFF;
// 		sync &= 0xFFFFFF;

// 	} while (sync != sync_target);

// 	uint64_t out = sync;
// 	ms = SYS_GetSysTickCnt();
// 	timeout = this->abort_window;

// 	for(uint8_t i=0; i<5;) {
// 		uint32_t ms2 = SYS_GetSysTickCnt();
// 		if (ms2 != ms) {
// 			// 1ms tick
// 			ms = ms2;
// 			timeout--;
// 		}
// 		if (!timeout) return 0;

// 		int16_t res = serial_read();
// 		if (res < 0) continue;

// 		out <<= 8;
// 		out |= res & 0xFF;

// 		i++;
// 	}

// 	while (serial_available() > 0) serial_read(); // Flush

// 	return out;
// }