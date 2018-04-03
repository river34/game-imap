//
//  Trait.h
//  GameIMap
//
//  Created by River Liu on 3/4/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include "IMap.h"
#include "../game-math/Vector2.h"
#include "../game-math/Vector3.h"
#include "../rapidxml/rapidxml.hpp"

class Blackboard;
using namespace std;
namespace GameIMap
{
	class Trait
	{
	public:
		typedef Trait* (*createInstanceFn) ();

	public:
		Trait() { }
		inline virtual void updateInterestMap(InterestMap* _interest, LayerMapCollection* const _collection, int _centerX, int _centerY, int _radius, int _id) { }
		inline virtual Vector2 getLocation(InterestMap* _interest, LayerMapCollection* const _collection, int _centerX, int _centerY, int _radius, int _id) { return Vector2::zero(); }
		inline virtual void onUpdate(Blackboard* _blackboard) { }
		inline virtual Vector3 getWorldLocation(Vector2 _location, int _centerX, int _centerY, InterestMap* _interest, InfluenceMap* _influence, float _cellSize, float _z) { return Vector3::zero(); }
		inline static Trait* create() { return new Trait(); }
	};

	class TraitFactory
	{
	private:
		TraitFactory(const TraitFactory&) { }
		TraitFactory& operator=(const TraitFactory&) { return *this; }
		typedef std::map<std::string, Trait::createInstanceFn> FactoryMap;
		FactoryMap m_FactoryMap;

	public:
		TraitFactory() { }
		~TraitFactory() { m_FactoryMap.clear(); }
		void registerClass(const std::string& name, Trait::createInstanceFn pfnCreate)
		{
			m_FactoryMap[name] = pfnCreate;
		}
		Trait* createInstance(const rapidxml::xml_node<>* node)
		{
			std::string id = (node->first_attribute("id") == nullptr) ? "" : node->first_attribute("id")->value();
			std::string name = node->name();
			FactoryMap::iterator it = m_FactoryMap.find(id + name);
			if (it != m_FactoryMap.end())
			{
				Trait* trait = it->second();
				return trait;
			}
			return nullptr;
		}
	};
}