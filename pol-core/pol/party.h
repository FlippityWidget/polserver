/*
History
=======


Notes
=======

*/

#ifndef PARTY_H
#define PARTY_H

#include "pktdef.h"
#include "syshook.h"
#include "proplist.h"
#include <vector>
namespace Pol {
  namespace Clib {
    class StreamWriter;
  }
  namespace Mobile {
	class Character;
  }
  namespace Network {
	class Client;
  }
  namespace Module {
    class EPartyRefObjImp;
  }

  namespace Core {
	class NPC;
    struct PKTBI_BF;
    struct PKTBI_6C;

	void handle_add( Network::Client* client, PKTBI_BF* msg );
	void handle_remove( Network::Client* client, PKTBI_BF* msg );
	void handle_member_msg( Network::Client* client, PKTBI_BF* msg );
	void handle_party_msg( Network::Client* client, PKTBI_BF* msg );
	void handle_loot_perm( Network::Client* client, PKTBI_BF* msg );
	void handle_accept_invite( Network::Client* client, PKTBI_BF* msg );
	void handle_decline_invite( Network::Client* client, PKTBI_BF* msg );

	class Party : public ref_counted
	{
	public:
	  explicit Party( Clib::ConfigElem& elem );
	  explicit Party( u32 serial );
	  bool is_leader( u32 serial ) const;
      bool is_member( u32 serial ) const;
      bool is_candidate( u32 serial ) const;
	  bool register_with_members();
	  bool add_candidate( u32 serial );
	  bool add_member( u32 serial );
	  void add_offline_mem( u32 serial );
	  bool remove_candidate( u32 serial );
	  bool remove_member( u32 serial );
	  bool remove_offline_mem( u32 serial );
	  void set_leader( u32 serial );
	  void send_remove_member( Mobile::Character* chr, bool* disband );
	  void send_member_list( Mobile::Character* to_chr );
	  void send_msg_to_all( unsigned int clilocnr, const char* affix = 0, Mobile::Character* exeptchr = NULL ) const;
	  bool test_size() const;
	  bool can_add() const;
	  void disband();
	  void send_stat_to( Mobile::Character* chr, Mobile::Character* bob ) const;
	  void send_stats_on_add( Mobile::Character* newmember ) const;
	  void on_mana_changed( Mobile::Character* chr ) const;
	  void on_stam_changed( Mobile::Character* chr ) const;
	  void send_member_msg_public( Mobile::Character* chr, u16* wtext, size_t wtextlen ) const;
	  void send_member_msg_private( Mobile::Character* chr, Mobile::Character* tochr, u16* wtext, size_t wtextlen ) const;
	  u32 leader() const;
	  void printOn( Clib::StreamWriter& sw ) const;
	  u32 get_member_at( unsigned short pos ) const;
	  void cleanup();

	  friend class Module::EPartyRefObjImp;

	private:
	  std::vector<u32> _member_serials;
      std::vector<u32> _candidates_serials;
      std::vector<u32> _offlinemember_serials;
	  u32 _leaderserial;
	  PropertyList _proplist;
	};

	typedef ref_ptr<Party> PartyRef;

    typedef std::vector <PartyRef> Parties;
	extern Parties parties;

	struct Party_Cfg
	{
	  struct
	  {
		unsigned short MaxPartyMembers;
		bool TreatNoAsPrivate;
		unsigned short DeclineTimeout;
		u16 PrivateMsgPrefix[SPEECH_MAX_LEN + 1];
		unsigned char PrivateMsgPrefixLen;
		bool RemoveMemberOnLogoff;
		bool RejoinPartyOnLogon;
	  } General;

	  struct
	  {
		ExportedFunction* CanAddToParty;     // (leader,member)
		ExportedFunction* CanLeaveParty;     // (member
		ExportedFunction* CanRemoveMember;   // (leader,member)
		ExportedFunction* ChangePrivateChat; // (member,tomember,uctext)
		ExportedFunction* ChangePublicChat;  // (member,uctext)
		ExportedFunction* OnAddToParty;      // (who)
		ExportedFunction* OnDecline;         // (who)
		ExportedFunction* OnDisband;         // (partyref)
		ExportedFunction* OnLeaveParty;      // (who,fromwho)
		ExportedFunction* OnLootPermChange;  // (who)
		ExportedFunction* OnPartyCreate;     // (party)
		ExportedFunction* OnPrivateChat;     // (member,tomember,uctext)
		ExportedFunction* OnPublicChat;      // (member,uctext)
	  } Hooks;

	};
	extern Party_Cfg party_cfg;

	const int CLP_Rejoined = 1005437; // You have rejoined the party.
	const int CLP_Add_Yourself = 1005439; // You cannot add yourself to a party.
	const int CLP_Already_Your_Party = 1005440; // This person is already in your party!
	const int CLP_Already_in_a_Party = 1005441; // This person is already in a party!
	const int CLP_Add_Living = 1005442; // You may only add living things to your party!
	const int CLP_Ignore_Offer = 1005444; // The creature ignores your offer.
	const int CLP_Added = 1005445; // You have been added to the party.
	const int CLP_Cannot_Remove_Self = 1005446; // You may only remove yourself from a party if you are not the leader.
	const int CLP_Allow_Loot = 1005447; // You have chosen to allow your party to loot your corpse.
	const int CLP_Prevent_Loot = 1005448; // You have chosen to prevent your party from looting your corpse.
	const int CLP_Disbanded = 1005449; // Your party has disbanded.
	const int CLP_Last_Person = 1005450; // The last person has left the party...
	const int CLP_Removed = 1005451; // You have been removed from the party.
	const int CLP_Player_Removed = 1005452; // A player has been removed from your party.
	const int CLP_Add_No_Leader = 1005453; // You may only add members to the party if you are the leader.
	const int CLP_Who_To_Add = 1005454; // Who would you like to add to your party?
	const int CLP_Who_to_Remove = 1005455; // Who would you like to remove from your party?
	const int CLP_Player_Rejoined = 1008087; // : rejoined the party.
	const int CLP_Invite = 1008089; // : You are invited to join the party. Type /accept to join or /decline to decline the offer.
	const int CLP_Invited = 1008090; // You have invited them to join the party.
	const int CLP_Notify_Decline = 1008091; // : Does not wish to join the party.
	const int CLP_Decline = 1008092; // You notify them that you do not wish to join the party.
	const int CLP_Joined = 1008094; // : joined the party.
	const int CLP_Max_Size = 1008095; // You may only have 10 in your party (this includes candidates).
	const int CLP_No_Party = 3000211; // You are not in a party.
	const int CLP_No_Invite = 3000222; // No one has invited you to be in a party.
	const int CLP_Too_Late_Decline = 3000223; // Too late to decline, you are already in the party. Try /quit.



	void send_invite( Mobile::Character* member, Mobile::Character* leader );
	void handle_add_member_cursor( Mobile::Character* chr, PKTBI_6C* msgin );
	void handle_remove_member_cursor( Mobile::Character* chr, PKTBI_6C* msgin );
	void add_candidate( Mobile::Character* member, Mobile::Character* leader );
	void disband_party( u32 leader );
	void send_empty_party( Mobile::Character* chr );
	void on_loggon_party( Mobile::Character* chr );
	void on_loggoff_party( Mobile::Character* chr );
	void send_attributes_normalized( Mobile::Character* chr, Mobile::Character* bob );
	void invite_timeout( Mobile::Character* mem );
  }
}
#endif
