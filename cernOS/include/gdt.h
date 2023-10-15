#ifndef __CERNOS__GDT_H
#define __CERNOS__GDT_H
#include <common/types.h>
namespace cernos{
		class GlobalDescriptorTable
		{
			public:
				class SegmentDescriptor
				{
					private:
						cernos::common::uint16_t limit_low;
						cernos::common::uint16_t base_low;
						cernos::common::uint8_t base_high;
						cernos::common::uint8_t access;
						cernos::common::uint8_t limit_high;
						cernos::common::uint8_t base_vhi;
					public:
						SegmentDescriptor(cernos::common::uint32_t base, cernos::common::uint32_t limit, cernos::common::uint8_t access);
						cernos::common::uint32_t Base();
						cernos::common::uint32_t Limit();
				} __attribute__((packed));
			public:
				SegmentDescriptor nullSegmentSelector;
				SegmentDescriptor freeSegmentSelector;
				SegmentDescriptor codeSegmentSelector;
				SegmentDescriptor dataSegmentSelector;
				GlobalDescriptorTable();
				~GlobalDescriptorTable();
				cernos::common::uint16_t CodeSegmentSelector();
				cernos::common::uint16_t DataSegmentSelector();

		}__attribute__((packed));
}


#endif