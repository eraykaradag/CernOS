#ifndef __CERNOS__DRIVERS__DRIVER_H
#define __CERNOS__DRIVERS__DRIVER_H

namespace cernos{
	namespace drivers{
		class Driver{
			public: 
				Driver();
				~Driver();

				virtual void Activate();
				virtual int Reset();
				virtual void Deactivate(); 
		};

		class DriverManager{
		private:
			Driver* drivers[255];
			int numDrivers;
		public:
			DriverManager(int num);
			void AddDriver(Driver*);

			void ActivateAll();

		};
	}
}
	

#endif