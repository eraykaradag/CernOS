#ifndef __GDT_H
#define __GDT_H

#include "types.h"

	class GlobalDescriptorTable
	{
		public:
			class SegmentDescriptor
			{
				private:
					uint16_t limit_low;
					uint16_t base_low;
					uint8_t base_high;
					uint8_t access;
					uint8_t limit_high;
					uint8_t flags;
					uint8_t base_vhi;
				public:
					SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access);
					uint32_t Base();
					uint32_t Limit();
			} __attribute__((packed));

			SegmentDescriptor nullSegmentSelector;
			SegmentDescriptor freeSegmentSelector;
			SegmentDescriptor codeSegmentSelector;
			SegmentDescriptor dataSegmentSelector;
		public:
			GlobalDescriptorTable();
			~GlobalDescriptorTable();
			uint16_t CodeSegmentSelector();
			uint16_t DataSegmentSelector();

	};

#endif