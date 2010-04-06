/*
History
=======

Notes
=======

*/

#ifndef POL_PACKETS_H
#define POL_PACKETS_H

#include <climits>
#include "../../clib/stl_inc.h"

#include "../../clib/endian.h"
#include "../../clib/singleton.h"
#include "../../clib/rawtypes.h"
#include "../layers.h"
#include "../pktoutid.h"
#include "../pktbothid.h"
#include "../pktdef.h"
#include "../ucfg.h"

#define MAX_PACKETS_INSTANCES 100

//interface for packets
class PacketInterface
{
	public:
		PacketInterface(){};
		virtual ~PacketInterface(){};
	public:
		u16 offset;
	public:
		virtual void ReSetBuffer() {};
		virtual char* getBuffer() { return NULL; }
		virtual inline u8 getID() { return 0; }
		virtual inline u16 getSubID() { return 0; }
};

typedef queue<PacketInterface*> PacketInterfaceQueue;
typedef std::map<u16, PacketInterfaceQueue> PacketInterfaceQueueMap;
typedef pair<u16, PacketInterfaceQueue> PacketInterfaceQueuePair;

// interface for the two different types of packetqueues ("normal" packets and packets with subs)
class PacketQueue
{
	public:
		PacketQueue(){};
		virtual ~PacketQueue(){};
	public:
		virtual PacketInterface* GetNext(u8 id, u16 sub=0) {return NULL;}
		virtual void Add(PacketInterface* pkt){};
		virtual void LogStatus(){};
};

// "normal" packet queue
class PacketQueueSingle : public PacketQueue
{
	public:
		PacketQueueSingle(){};
		~PacketQueueSingle(){};
	private:
		PacketInterfaceQueue packets;
	public:
		PacketInterface* GetNext(u8 id, u16 sub=0);
		void Add(PacketInterface* pkt);
		void LogStatus();
};

// packet with subs queue
class PacketQueueSubs : public PacketQueue
{
	public:
		PacketQueueSubs(){};
		~PacketQueueSubs(){};
	private:
		PacketInterfaceQueueMap packets;
	public:
		PacketInterface* GetNext(u8 id, u16 sub=0);
		void Add(PacketInterface* pkt);
		void LogStatus();
};

typedef pair<u8, PacketQueue*> PacketQueuePair;
typedef std::map<u8, PacketQueue*> PacketQueueMap;

// singleton "holder" of packets !EntryPoint!
class PacketsSingleton
{
	public:
		PacketsSingleton();
		~PacketsSingleton(){};
	private:
		PacketQueueMap packets;
	public:
		PacketInterface* getPacket(u8 id, u16 sub=0);
		void ReAddPacket(PacketInterface* pkt);
		void LogStatus();
};

// the real definition
typedef Singleton<PacketsSingleton> Packets;

//wierdo generic template definitions for packets

// "writer"class
template <u8 _id, u16 _size>
class PacketWriter : public PacketInterface
{
	public:
		PacketWriter() { ReSetBuffer(); }
		char buffer[_size];
		void ReSetBuffer() { memset(buffer,0,sizeof(buffer)); buffer[0]=_id; offset=1; }
		char* getBuffer() { return buffer; }
		inline u8 getID() { return buffer[0]; }

		void Write(u32 x)
		{
			(*(u32*)&buffer[offset]) = x;
			offset += 4;
		};
		void Write(s32 x)
		{
			(*(s32*)&buffer[offset]) = x;
			offset += 4;
		};
		void Write(u16 x)
		{
			(*(u16*)&buffer[offset]) = x;
			offset += 2;
		};
		void Write(s16 x)
		{
			(*(s16*)&buffer[offset]) = x;
			offset += 2;
		};
		void Write(u8 x) { buffer[offset++] = x; };
		void Write(s8 x) { buffer[offset++] = x; };
		void WriteFlipped(u32 x)
		{
			(*(u32*)&buffer[offset]) = cfBEu32(x);
			offset += 4;
		};
		void WriteFlipped(s32 x)
		{
			(*(s32*)&buffer[offset]) = cfBEu32(x);
			offset += 4;
		};
		void WriteFlipped(u16 x)
		{
			(*(u16*)&buffer[offset]) = cfBEu16(x);
			offset += 2;
		};
		void WriteFlipped(s16 x)
		{
			(*(s16*)&buffer[offset]) = cfBEu16(x);
			offset += 2;
		};
		void Write(const char* x, u16 len, bool nullterm=true)
		{
			strncpy(&buffer[offset], x, nullterm ? len-1 : len);
			offset += len;
		}
		void Write(const u16* x, u16 len, bool nullterm=true)
		{
			u16* _buffer = ((u16*)&buffer[offset]);
			offset += len*2;
			while (len-- > 0)
			{
				*(_buffer++) = *x++;
			}
			if (nullterm)
				offset += 2;
		}
		void WriteFlipped(const u16* x, u16 len, bool nullterm=true)
		{
			u16* _buffer = ((u16*)&buffer[offset]);
			offset += len*2;
			while (len-- > 0)
			{
				*(_buffer++) = ctBEu16(*x);
				++x;
			}
			if (nullterm)
				offset += 2;
		}
};

// "normal" pkt
template <u8 _id, u16 _size>
class PacketTemplate : public PacketWriter<_id, _size>
{
};

// sub packet
template <u8 _id, u16 _sub, u16 _size>
class PacketTemplateSub : public PacketWriter<_id, _size>
{
	private:
		u16 sub;
	public:
		inline u16 getSubID() { return sub; }
};

// creates new packets
PacketInterface* GetPacket(u8 id, u16 sub=0);

#define REQUESTPACKET(_pkt,_id) static_cast<_pkt*>(Packets::instance()->getPacket(_id))
#define READDPACKET(_msg) Packets::instance()->ReAddPacket(_msg)

// Packet defs start
typedef PacketTemplate<PKTOUT_0B_ID,7> PktOut_0B;
typedef PacketTemplate<PKTOUT_11_ID,91> PktOut_11;
typedef PacketTemplate<PKTOUT_17_ID,12> PktOut_17;
typedef PacketTemplate<PKTOUT_1A_ID,20> PktOut_1A;
typedef PacketTemplate<PKTOUT_1B_ID,37> PktOut_1B;
typedef PacketTemplate<PKTOUT_1C_ID,44 + SPEECH_MAX_LEN + 1> PktOut_1C;
typedef PacketTemplate<PKTOUT_1D_ID,5> PktOut_1D;
typedef PacketTemplate<PKTOUT_20_ID,19> PktOut_20;
typedef PacketTemplate<PKTOUT_21_ID,8> PktOut_21;
typedef PacketTemplate<PKTBI_22_APPROVED_ID,3> PktBi_22;
typedef PacketTemplate<PKTOUT_24_ID,7> PktOut_24;
typedef PacketTemplate<PKTOUT_25_ID,21> PktOut_25;
typedef PacketTemplate<PKTOUT_27_ID,2> PktOut_27;
typedef PacketTemplate<PKTOUT_29_ID,1> PktOut_29;
typedef PacketTemplate<PKTOUT_2D_ID,17> PktOut_2D;
typedef PacketTemplate<PKTOUT_2E_ID,15> PktOut_2E;
typedef PacketTemplate<PKTOUT_2F_ID,10> PktOut_2F;
typedef PacketTemplate<PKTOUT_3C_ID,5 + MAX_CONTAINER_ITEMS * 20> PktOut_3C;
typedef PacketTemplate<PKTOUT_4F_ID,2> PktOut_4F;
typedef PacketTemplate<PKTOUT_53_ID,2> PktOut_53;
typedef PacketTemplate<PKTOUT_54_ID,12> PktOut_54;
typedef PacketTemplate<PKTOUT_55_ID,1> PktOut_55;
typedef PacketTemplate<PKTOUT_65_ID,4> PktOut_65;
typedef PacketTemplate<PKTOUT_6D_ID,3> PktOut_6D;
typedef PacketTemplate<PKTOUT_6E_ID,14> PktOut_6E;
typedef PacketTemplate<PKTOUT_70_ID,28> PktOut_70;
typedef PacketTemplate<PKTOUT_74_ID,0xFFFF> PktOut_74;
typedef PacketTemplate<PKTOUT_76_ID,16> PktOut_76;
typedef PacketTemplate<PKTOUT_77_ID,17> PktOut_77;
typedef PacketTemplate<PKTOUT_78_ID,19 + (9 * HIGHEST_LAYER) + 4> PktOut_78;
typedef PacketTemplate<PKTOUT_7C_ID,2000> PktOut_7C;
typedef PacketTemplate<PKTOUT_82_ID,2> PktOut_82;
typedef PacketTemplate<PKTOUT_86_ID,66> PktOut_86;
typedef PacketTemplate<PKTOUT_89_ID,7+(5*(NUM_LAYERS+1))+1> PktOut_89;
typedef PacketTemplate<PKTOUT_8C_ID,11> PktOut_8C;
typedef PacketTemplate<PKTOUT_90_ID,19> PktOut_90;
typedef PacketTemplate<PKTOUT_9E_ID,0xFFFF> PktOut_9E;
typedef PacketTemplate<PKTOUT_A1_ID,9> PktOut_A1;
typedef PacketTemplate<PKTOUT_A2_ID,9> PktOut_A2;
typedef PacketTemplate<PKTOUT_A3_ID,9> PktOut_A3;
typedef PacketTemplate<PKTOUT_A5_ID,URL_MAX_LEN+4> PktOut_A5;
typedef PacketTemplate<PKTOUT_A6_ID,10010> PktOut_A6;
typedef PacketTemplate<PKTOUT_A8_ID,2000> PktOut_A8;

typedef PacketTemplate<PKTOUT_AE_ID,((SPEECH_MAX_LEN) + 1)*2+48> PktOut_AE;
typedef PacketTemplateSub<PKTBI_BF_ID,0x4,12> Pktout_bf_sub4_closegump;
// Packet defs end


#endif
