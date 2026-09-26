#ifndef UTILS_HOOK_MIPS_HPP
#define UTILS_HOOK_MIPS_HPP

#include <ml/types.h>

namespace mipse {

	enum RegisterID {
		r0 = 0,
		r1,
		r2,
		r3,
		r4,
		r5,
		r6,
		r7,
		r8,
		r9,
		r10,
		r11,
		r12,
		r13,
		r14,
		r15,
		r16,
		r17,
		r18,
		r19,
		r20,
		r21,
		r22,
		r23,
		r24,
		r25,
		r26,
		r27,
		r28,
		r29,
		r30,
		r31,
		zero = r0,
		at = r1,
		v0 = r2,
		v1 = r3,
		a0 = r4,
		a1 = r5,
		a2 = r6,
		a3 = r7,
		t0 = r8,
		t1 = r9,
		t2 = r10,
		t3 = r11,
		t4 = r12,
		t5 = r13,
		t6 = r14,
		t7 = r15,
		s0 = r16,
		s1 = r17,
		s2 = r18,
		s3 = r19,
		s4 = r20,
		s5 = r21,
		s6 = r22,
		s7 = r23,
		t8 = r24,
		t9 = r25,
		k0 = r26,
		k1 = r27,
		gp = r28,
		sp = r29,
		fp = r30,
		ra = r31,
		InvalidGPRReg = -1,
	};

	enum SPRegister { fir = 0,
					  fccr = 25,
					  fexr = 26,
					  fenr = 28,
					  fcsr = 31,
					  pc };

	enum FPRegisterID {
		f0,
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,
		f13,
		f14,
		f15,
		f16,
		f17,
		f18,
		f19,
		f20,
		f21,
		f22,
		f23,
		f24,
		f25,
		f26,
		f27,
		f28,
		f29,
		f30,
		f31,
		InvalidFPRReg = -1,
	};

	enum {
		OP_SH_RD = 11,
		OP_SH_RT = 16,
		OP_SH_RS = 21,
		OP_SH_SHAMT = 6,
		OP_SH_CODE = 16,
		OP_SH_FD = 6,
		OP_SH_FS = 11,
		OP_SH_FT = 16,
		OP_SH_MSB = 11,
		OP_SH_LSB = 6
	};

	inline u32 nop() {
		return (0x00000000);
	}
	inline u32 bc0f(i32 offset) {
		return (0x4100FF00 | (offset & 0xffff));
	}
	inline u32 di() {
		return (0x42000039);
	}
	inline u32 div1(RegisterID rs, RegisterID rt) {
		return (0x7000001a | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 ei() {
		return (0x42000038);
	}
	inline u32 eret() {
		return (0x42000018);
	}
	inline u32 madd(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000000 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 maddu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000001 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 mfhi1(RegisterID rd) {
		return (0x70000010 | (rd << OP_SH_RD));
	}
	inline u32 mflo1(RegisterID rd) {
		return (0x70000012 | (rd << OP_SH_RD));
	}
	inline u32 mfsa(RegisterID rd) {
		return (0x00000028 | (rd << OP_SH_RD));
	}
	inline u32 mthi1(RegisterID rs) {
		return (0x70000011 | (rs << OP_SH_RS));
	}
	inline u32 mtlo1(RegisterID rs) {
		return (0x70000013 | (rs << OP_SH_RS));
	}
	inline u32 mtsa(RegisterID rs) {
		return (0x00000029 | (rs << OP_SH_RS));
	}
	inline u32 mtsab(RegisterID rs, i32 imm) {
		return (0x04180000 | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 mult(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000018 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 mult1(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000018 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 multu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000019 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 multu1(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000019 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 _sync() {
		return (0x0000000f);
	}
	inline u32 syscall() {
		return (0x0000000c);
	}
	inline u32 tge(RegisterID rs, RegisterID rt) {
		return (0x0000bb30 | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 tlbwi() {
		return (0x42000002);
	}
	inline u32 abss(FPRegisterID fd, FPRegisterID fs) {
		return (0x46000005 | (fd << OP_SH_FD) | (fs << OP_SH_FS));
	}
	inline u32 adds(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000000 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 addas(FPRegisterID fs, FPRegisterID ft) {
		return (0x46000018 | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 bc1f(i32 offset) {
		return (0x45000000 | (offset & 0xffff));
	}
	inline u32 bc1fl(i32 offset) {
		return (0x45020000 | (offset & 0xffff));
	}
	inline u32 bc1tl(i32 offset) {
		return (0x45030000 | (offset & 0xffff));
	}
	inline u32 ceqs(FPRegisterID fs, FPRegisterID ft) {
		return (0x46000032 | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 cvtsw(FPRegisterID fd, FPRegisterID fs) {
		return (0x46800020 | (fd << OP_SH_FD) | (fs << OP_SH_FS));
	}
	inline u32 cvtws(FPRegisterID fd, FPRegisterID fs) {
		return (0x46000024 | (fd << OP_SH_FD) | (fs << OP_SH_FS));
	}
	inline u32 divs(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000003 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 madds(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x4600001c | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 maddas(FPRegisterID fs, FPRegisterID ft) {
		return (0x4600001e | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 maxs(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000028 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 mfc1(RegisterID rt, FPRegisterID fs) {
		return (0x44000000 | (rt << OP_SH_RT) | (fs << OP_SH_FS));
	}
	inline u32 mins(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000029 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 movs(FPRegisterID fd, FPRegisterID fs) {
		return (0x46000006 | (fd << OP_SH_FD) | (fs << OP_SH_FS));
	}
	inline u32 msubs(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x4600001d | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 msubas(FPRegisterID fs, FPRegisterID ft) {
		return (0x4600001f | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 mtc1(RegisterID rt, FPRegisterID fs) {
		return (0x44800000 | (rt << OP_SH_RT) | (fs << OP_SH_FS));
	}
	inline u32 muls(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000002 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 mulas(FPRegisterID fs, FPRegisterID ft) {
		return (0x4600001a | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 negs(FPRegisterID fd, FPRegisterID fs) {
		return (0x46000007 | (fd << OP_SH_FD) | (fs << OP_SH_FS));
	}
	inline u32 rsqrts(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000016 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 sqrts(FPRegisterID fd, FPRegisterID ft) {
		return (0x46000004 | (fd << OP_SH_FD) | (ft << OP_SH_FT));
	}
	inline u32 subs(FPRegisterID fd, FPRegisterID fs, FPRegisterID ft) {
		return (0x46000001 | (fd << OP_SH_FD) | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 paddh(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000108 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 paddsw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000408 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 paddw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000008 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pand(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000489 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pcgth(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000188 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pcpyh(RegisterID rd, RegisterID rt) {
		return (0x700006e9 | (rd << OP_SH_RD) | (rt << OP_SH_RT));
	}
	inline u32 pcpyld(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000389 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pcpyud(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700003a9 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pexch(RegisterID rd, RegisterID rt) {
		return (0x700006a9 | (rd << OP_SH_RD) | (rt << OP_SH_RT));
	}
	inline u32 pexew(RegisterID rd, RegisterID rt) {
		return (0x70000789 | (rd << OP_SH_RD) | (rt << OP_SH_RT));
	}
	inline u32 pextlb(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000688 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pextlh(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000588 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pextlw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000488 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pextub(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700006a8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pextuw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700004a8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pmaxh(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700001c8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pmaxw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700000c8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pmfhi(RegisterID rd) {
		return (0x70000209 | (rd << OP_SH_RD));
	}
	inline u32 pmflo(RegisterID rd) {
		return (0x70000249 | (rd << OP_SH_RD));
	}
	inline u32 pminh(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700001e8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pminw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700000e8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pmulth(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000709 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pnor(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700004e9 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 por(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700004a9 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 ppacb(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700006c8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 ppach(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700005c8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 prot3w(RegisterID rd, RegisterID rt) {
		return (0x700007c9 | (rd << OP_SH_RD) | (rt << OP_SH_RT));
	}
	inline u32 psllh(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x70000034 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 psllw(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x7000003c | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 psrah(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x70000037 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 psraw(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x7000003f | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 psrlh(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x70000036 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 psrlw(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x7000003e | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 psubb(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000248 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 psubw(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x70000048 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 pxor(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700004c9 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 qfsrv(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x700006e8 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 cles(FPRegisterID fs, FPRegisterID ft) {
		return (0x46000036 | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 clts(FPRegisterID fs, FPRegisterID ft) {
		return (0x46000034 | (fs << OP_SH_FS) | (ft << OP_SH_FT));
	}
	inline u32 add(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000020 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 addu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000021 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 _and(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000024 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 daddu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x0000002d | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 _div(RegisterID rs, RegisterID rt) {
		return (0x0000001a | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 divu(RegisterID rs, RegisterID rt) {
		return (0x0000001b | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 dsll(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x00000038 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 dsll32(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x0000003c | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 dsllv(RegisterID rd, RegisterID rt, RegisterID rs) {
		return (0x00000014 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | (rs << OP_SH_RS));
	}
	inline u32 dsra(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x0000003b | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 dsra32(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x0000003f | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 dsrl(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x0000003a | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 dsrl32(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x0000003e | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 dsrlv(RegisterID rd, RegisterID rt, RegisterID rs) {
		return (0x00000016 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | (rs << OP_SH_RS));
	}
	inline u32 dsubu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x0000002f | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 jalr(RegisterID rs) {
		return (0x0000f809 | (rs << OP_SH_RS));
	}
	inline u32 jr(RegisterID rs) {
		return (0x00000008 | (rs << OP_SH_RS));
	}
	inline u32 mfhi(RegisterID rd) {
		return (0x00000010 | (rd << OP_SH_RD));
	}
	inline u32 mflo(RegisterID rd) {
		return (0x00000012 | (rd << OP_SH_RD));
	}
	inline u32 movn(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x0000000b | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 movz(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x0000000a | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 mthi(RegisterID rs) {
		return (0x00000011 | (rs << OP_SH_RS));
	}
	inline u32 mtlo(RegisterID rs) {
		return (0x00000013 | (rs << OP_SH_RS));
	}
	inline u32 nor(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000027 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 _or(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000025 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 sll(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0000000000 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 sllv(RegisterID rd, RegisterID rt, RegisterID rs) {
		return (0x00000004 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | (rs << OP_SH_RS));
	}
	inline u32 slt(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x0000002a | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 sltu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x0000002b | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 sra(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x00000003 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 srav(RegisterID rd, RegisterID rt, RegisterID rs) {
		return (0x00000007 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | (rs << OP_SH_RS));
	}
	inline u32 srl(RegisterID rd, RegisterID rt, i32 shamt) {
		return (0x00000002 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | ((shamt & 0x1f) << OP_SH_SHAMT));
	}
	inline u32 srlv(RegisterID rd, RegisterID rt, RegisterID rs) {
		return (0x00000006 | (rd << OP_SH_RD) | (rt << OP_SH_RT) | (rs << OP_SH_RS));
	}
	inline u32 sub(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000022 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 subu(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000023 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 _xor(RegisterID rd, RegisterID rs, RegisterID rt) {
		return (0x00000026 | (rd << OP_SH_RD) | (rs << OP_SH_RS) | (rt << OP_SH_RT));
	}
	inline u32 addi(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x20000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 addiu(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x24000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 andi(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x30000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 daddi(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x60000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 daddiu(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x64000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 lb(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x80000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lbu(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x90000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 ld(RegisterID rt, RegisterID rs, i32 offset) {
		return (0xDC000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 ldl(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x68000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 ldr(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x6C000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lh(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x84000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lhu(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x94000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lui(RegisterID rt, i32 imm) {
		return (0x3c000000 | (rt << OP_SH_RT) | (*(i32*)&imm & 0xffff));
	}
	inline u32 lw(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x8c000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lwl(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x88000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lwr(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x98000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lwu(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x9C000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 ori(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x34000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 sq(RegisterID rt, RegisterID rs, i32 offset) {
		return (0x7C000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 slti(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x28000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 sltiu(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x2c000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 sb(RegisterID rt, RegisterID rs, i32 offset) {
		return (0xa0000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 sh(RegisterID rt, RegisterID rs, i32 offset) {
		return (0xa4000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 sw(RegisterID rt, RegisterID rs, i32 offset) {
		return (0xac000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 lwc1(FPRegisterID ft, RegisterID rs, i32 offset) {
		return (0xc4000000 | (ft << OP_SH_FT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 swc1(FPRegisterID ft, RegisterID rs, i32 offset) {
		return (0xe4000000 | (ft << OP_SH_FT) | (rs << OP_SH_RS) | (offset & 0xffff));
	}
	inline u32 xori(RegisterID rt, RegisterID rs, i32 imm) {
		return (0x38000000 | (rt << OP_SH_RT) | (rs << OP_SH_RS) | (*(i32*)&imm & 0xffff));
	}
	inline u32 li(RegisterID dest, i32 imm) {
		return (0x34000000 | (dest << OP_SH_RT) | (*(i32*)&imm & 0xffff));
	}
	inline u32 j(i32 address) {
		return ((0x08000000 | ((address & 0x0fffffff) >> 2)));
	}
	inline u32 jal(i32 address) {
		return ((0x0C000000 | ((address & 0x0fffffff) >> 2)));
	}
	inline u32 b(i32 imm) {
		return (0x10000000 | (imm & 0xffff));
	}
	inline u32 beq(RegisterID rs, RegisterID rt, i32 imm) {
		return (0x10000000 | (rs << OP_SH_RS) | (rt << OP_SH_RT) | (imm & 0xffff));
	}
	inline u32 bne(RegisterID rs, RegisterID rt, i32 imm) {
		return (0x14000000 | (rs << OP_SH_RS) | (rt << OP_SH_RT) | (imm & 0xffff));
	}
	inline u32 move(RegisterID rd, RegisterID rs) {
		/* addu */
		return (0x00000021 | (rd << OP_SH_RD) | (rs << OP_SH_RS));
	}
	inline u32 moveq(RegisterID rd, RegisterID rs) {
		return (0x700004a9 | (rd << OP_SH_RD) | (rs << OP_SH_RS));
	}

} // namespace mipse

#endif
