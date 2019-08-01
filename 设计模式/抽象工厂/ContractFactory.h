#pragma once
// concrete_factory.h
#ifndef CONCRETE_FACTORY_H
#define CONCRETE_FACTORY_H

#include "AbstractFactory.h"
#include "ContractProduct.h"

// 奔驰工厂
class BenzFactory : public AbstractFactory
{
public:
	ICar* CreateCar() override {
		return new BenzCar();
	}

	IBike* CreateBike() override {
		return new BenzBike();
	}
};

// 宝马工厂
class BmwFactory : public AbstractFactory
{
public:
	ICar* CreateCar() override {
		return new BmwCar();
	}

	IBike* CreateBike() override {
		return new BmwBike();
	}
};

// 奥迪工厂
class AudiFactory : public AbstractFactory
{
public:
	ICar* CreateCar() override {
		return new AudiCar();
	}

	IBike* CreateBike() override {
		return new AudiBike();
	}
};

#endif // CONCRETE_FACTORY_H