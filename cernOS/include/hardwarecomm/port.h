#ifndef __CERNOS__HARDWARECOMM__PORT_H
#define __CERNOS__HARDWARECOMM__PORT_H
#include <common/types.h>
namespace cernos{
	namespace hardwarecomm{
		
		
		class Port
		{
			protected:
				cernos::common::uint16_t portNum;
				Port(cernos::common::uint16_t portNum);
				~Port();
		};
		class Port8bit: public Port
		{
			public:
				Port8bit(cernos::common::uint16_t portNum);
				~Port8bit();

				virtual void Write(cernos::common::uint8_t data);
				virtual cernos::common::uint8_t Read();
		};
		class Port8bitSlow: public Port8bit
		{
			public:
				Port8bitSlow(cernos::common::uint16_t portNum);
				~Port8bitSlow();

				virtual void Write(cernos::common::uint8_t data);
		};
		class Port16bit: public Port
		{
			public:
				Port16bit(cernos::common::uint16_t portNum);
				~Port16bit();

				virtual void Write(cernos::common::uint16_t data);
				virtual cernos::common::uint16_t Read();
		};
		class Port32bit: public Port
		{
			public:
				Port32bit(cernos::common::uint16_t portNum);
				~Port32bit();

				virtual void Write(cernos::common::uint32_t data);
				virtual cernos::common::uint32_t Read();
		};
	}
}
	

	
#endif