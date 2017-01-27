#pragma once
#include "Packet.h"
#include <vector>

namespace PS {
	namespace Send {
		class LoadServerInfo {
		public:
			LoadServerInfo();
			Packet GetList() { return p; }
		private:
			Packet p;
			/* packet structure :
			int32_t packettype
			int32_t size of version
			string version
			int32_t How many servers
			int32_t size of db
			string db
			int32_tsize of name
			string name
			*/

		};
	}
	namespace Recv {

	}
}