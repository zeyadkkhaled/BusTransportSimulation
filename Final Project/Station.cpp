#include "DEFS.h"
#include "Station.h"
#include <iostream>

Station::Station()
{
	this->StationID = 0;
}
Station::Station(int ID)
{
	this->StationID = ID;
}


void Station::setStationID(int ID)
{
	this->StationID = ID;
}
int Station::getStationID() const
{
	return this->StationID;
}

void Station::PrintAllQueues() const
{
	std::cout << "Station: " << this->StationID << std::endl;
	std::cout << "FWD Direction Wheelchair Passenger" << std::endl;
	WheelPassengerQueueFWD.PrintPassengerQueue();
	std::cout << "FWD Direction Bus Passenger" << std::endl;
	PassengerPrioQueueFWD.PrintPassengerPrioQueue();
	std::cout << "BWD Direction Wheelchair Passenger" << std::endl;
	WheelPassengerQueueBWD.PrintPassengerQueue();
	std::cout << "BWD Direction Bus Passenger" << std::endl;
	PassengerPrioQueueBWD.PrintPassengerPrioQueue();
	std::cout << std::endl;
}
void Station::AddPassengerToQueue(Passenger* newps)
{
	if (newps->getEndStation() - newps->getStartStation() > 0)
	{
		if (newps->getPassengerType() == WP)
			WheelPassengerQueueFWD.enqueue(newps);
		else
			PassengerPrioQueueFWD.enqueue(newps->getPassengerType(), newps);
	}
	else
	{
		if (newps->getPassengerType() == WP)
			WheelPassengerQueueBWD.enqueue(newps);
		else
			PassengerPrioQueueBWD.enqueue(newps->getPassengerType(), newps);
	}
}
bool Station::RemovePassenger(int ID)
{
	if (!WheelPassengerQueueFWD.RemovePassenger(ID))
	{
		if (!WheelPassengerQueueBWD.RemovePassenger(ID))
		{
			if (!PassengerPrioQueueFWD.RemovePassenger(ID))
			{
				if (!PassengerPrioQueueBWD.RemovePassenger(ID))
					return false;
			}

		}
	}
	return true;
}

bool Station::RemoveNPSP(bool NormalOrNot, Passenger*& p)
{
	bool returned = true;
	if (!PassengerPrioQueueFWD.RemoveFirstTypePassenger(p, NormalOrNot))
		returned = PassengerPrioQueueBWD.RemoveFirstTypePassenger(p, NormalOrNot);
	return returned;
}
bool Station::RemoveWP(Passenger*& p)
{
	bool returned = true;
	if (!WheelPassengerQueueFWD.RemoveFirstPassenger(p))
		returned = WheelPassengerQueueBWD.RemoveFirstPassenger(p);
	return returned;
}

void Station::BusArrived(Bus* bus)
{
	bus->IncrementCurrentStation();
	if (bus->getDirection() == FWD)
		busFWD.enqueue(bus);
	else
		busBWD.enqueue(bus);
}

void Station::IncrementQueuesWaitingTime(int TimeStepInSeconds,int maxWaitingTime, int SID)
{
	PassengerPrioQueueFWD.IncrementPassengerWT(TimeStepInSeconds,maxWaitingTime);
	PassengerPrioQueueBWD.IncrementPassengerWT(TimeStepInSeconds,maxWaitingTime);
	WheelPassengerQueueFWD.IncrementPassengerWT(TimeStepInSeconds);
	WheelPassengerQueueBWD.IncrementPassengerWT(TimeStepInSeconds);

	busFWD.IncrementTimeInStation(TimeStepInSeconds, SID);
	busFWD.IncrementTimeInStation(TimeStepInSeconds, SID);
}
void Station::CheckBussesInStationZero(TimeHM TS)
{
	
}

void Station::DequeueBusS0(Bus*& b)
{
	busFWD.dequeue(b);
}

int Station::CountSizeSP()
{
	return PassengerPrioQueueBWD.CountSize(false) + PassengerPrioQueueFWD.CountSize(false);
}
int Station::CountSizeNP()
{
	return PassengerPrioQueueBWD.CountSize(true) + PassengerPrioQueueFWD.CountSize(true);
}
int Station::CountSizeWP()
{
	return WheelPassengerQueueFWD.CountSize() + WheelPassengerQueueBWD.CountSize();
}

int Station::CountMBBusFWD()
{
	return busFWD.CountMBBus();
}
int Station::CountWBBusFWD()
{
	return busFWD.CountWBBus();
}
int Station::CountMBBusBWD()
{
	return busBWD.CountMBBus();
}
int Station::CountWBBusBWD()
{
	return busBWD.CountWBBus();
}
int Station::CountPassengerOnBusFWD(int busid)
{
	return busFWD.CountPassengerOnBus(busid);
}
int Station::CountPassengerOnBusBWD(int busid)
{
	return busBWD.CountPassengerOnBus(busid);
}

bool Station::PeekWheelFWD(int& ID, int index)
{
	return WheelPassengerQueueFWD.PeekSpecific(ID, index);
}
bool Station::PeekWheelBWD(int& ID, int index)
{
	return WheelPassengerQueueBWD.PeekSpecific(ID, index);
}
bool Station::PeekSPNPFWD(int& ID, int& prio, int index)
{
	return PassengerPrioQueueFWD.PeekSpecific(ID, prio, index);
}
bool Station::PeekSPNPBWD(int& ID, int& prio, int index)
{
	return PassengerPrioQueueBWD.PeekSpecific(ID, prio, index);
}

bool Station::PeekFWDBus(int& ID, bool& Type, int& CurrentCapacity ,int TotalCapacity, int index)
{
	return busFWD.PeekBus(ID, Type, CurrentCapacity, TotalCapacity, index);
}
bool Station::PeekBWDBus(int& ID, bool& Type, int& CurrentCapacity, int TotalCapacity, int index)
{
	return busBWD.PeekBus(ID, Type, CurrentCapacity, TotalCapacity, index);
}
bool Station::PeekPassengerOnBusFWD(int busid, int& PID, int index)
{
	return busFWD.PeekPassengerOnBus(busid, PID, index);
}
bool Station::PeekPassengerOnBusBWD(int busid, int& PID, int index)
{
	return busBWD.PeekPassengerOnBus(busid, PID, index);
}

bool Station::isEmptyStation() const
{
	if (!PassengerPrioQueueFWD.isEmpty())
		return false;
	else if (!PassengerPrioQueueBWD.isEmpty())
		return false;
	else if (!WheelPassengerQueueFWD.isEmpty())
		return false;
	else if (!WheelPassengerQueueBWD.isEmpty())
		return false;
	else
		return true;
}

bool Station::DropPassengerFromBusFWD(Passenger* & p, TimeHM t, int rst)
{
	Bus* dummy = new Bus();		//This dummy is used to drop passengers
	Passenger* ps = nullptr;
	if (busFWD.peek(dummy))
	{
		if (dummy->DropPassenger(ps, t, rst))
			p = ps;
		else
			return false;
	}
	else
		return false;
	return true;
}
bool Station::DropPassengerFromBusBWD(Passenger* & p, TimeHM t, int rst)
{
	Bus* dummy = new Bus();    //This dummy is used to drop passengers
	Passenger* ps = new Passenger();
	if (busBWD.peek(dummy))
	{
		if (dummy->DropPassenger(ps, t, rst))
			p = ps;
		else
			return false;
	}
	else
		return false;
	return true;
}
bool Station::AddPassengerToBusFWD(int CapacityMB, int CapacityWB, Bus*& b, TimeHM t, int rst)
{
	Bus* dummy = new Bus();
	busFWD.peek(dummy);
	if (dummy->getBusType() == MB)
	{
		if (dummy->getAvailablePassenger(CapacityMB) <= 0)
		{
			busFWD.dequeue(dummy);
			b = dummy;
			return false;
		}

		Passenger* pass = new Passenger();
		int dummyprio = 0;

		if (PassengerPrioQueueFWD.dequeue(dummyprio, pass))
		{
			dummy->AddPassenger(CapacityMB, pass, t, rst);
			return true;
		}
		else
		{
			busFWD.dequeue(dummy);
			b = dummy;
			return false;
		}	
	}
	else
	{
		if (dummy->getAvailablePassenger(CapacityWB) <= 0)
		{
			busFWD.dequeue(dummy);
			b = dummy;
			return false;
		}
		Passenger* pass = new Passenger();
		int dummyprio = 0;

		if (WheelPassengerQueueFWD.dequeue(pass))
		{
			dummy->AddPassenger(CapacityWB, pass, t, rst);
			return true;
		}
		else
		{
			busFWD.dequeue(dummy);
			b = dummy;
			return false;
		}
	}
}
bool Station::AddPassengerToBusBWD(int CapacityMB, int CapacityWB, Bus*& b, TimeHM t, int rst)
{
	Bus* dummy = new Bus();
	busBWD.peek(dummy);
	if (dummy->getBusType() == MB)
	{
		if (dummy->getAvailablePassenger(CapacityMB) <= 0)
		{
			busBWD.dequeue(dummy);
			b = dummy;
			return false;
		}

		Passenger* pass = new Passenger();
		int dummyprio = 0;

		if (PassengerPrioQueueBWD.dequeue(dummyprio, pass))
		{
			dummy->AddPassenger(CapacityMB, pass, t, rst);
			return true;
		}
		else
		{
			busBWD.dequeue(dummy);
			b = dummy;
			return false;
		}
	}
	else
	{
		if (dummy->getAvailablePassenger(CapacityWB) <= 0)
		{
			busBWD.dequeue(dummy);
			b = dummy;
			return false;
		}
		Passenger* pass = new Passenger();
		int dummyprio = 0;

		if (WheelPassengerQueueBWD.dequeue(pass))
		{
			dummy->AddPassenger(CapacityWB, pass, t, rst);
			return true;
		}
		else
		{
			busBWD.dequeue(dummy);
			b = dummy;
			return false;
		}
	}
}

Station::~Station()
{

}
