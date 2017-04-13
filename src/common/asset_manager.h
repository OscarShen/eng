/************************************************************************ 
 * @description :  资源管理
 * @author		:  Oscar Shen
 * @creat 		:  2017年3月29日20:27:37
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ENG_ASSET_MANAGER_H_
#define ENG_ASSET_MANAGER_H_
#include <core/eng.h>
namespace eng {

	class AssetManager
	{
	public:
		int counter = 0;
		std::map<void *, int> asset_to_id;
		std::map<int, std::weak_ptr<void>> id_to_asset;

	public:
		static AssetManager &get_instance() {
			static AssetManager manager;
			return manager;
		}

		// 非线程安全
		template<typename T>
		std::shared_ptr<T> get_asset_(int id) {
			CHECK_INFO(id_to_asset.find(id) != id_to_asset.end(), "Asset not found!");
			auto ptr = id_to_asset[id];
			CHECK_INFO(!ptr.expired(), "Asset has been expired");
			return std::static_pointer_cast<T>(ptr.lock());
		}

		template<typename T>
		int insert_asset_(const std::shared_ptr<T> &ptr) {
			CHECK_INFO(asset_to_id.find(ptr.get()) == asset_to_id.end(), "Asset already exists");
			int id = counter++;
			id_to_asset[id] = static_cast<std::weak_ptr<void>>(std::weak_ptr<T>(ptr));
			asset_to_id[ptr.get()] = id;
			return id;
		}

		template<typename T>
		static std::shared_ptr<T> get_asset(int id) {
			return get_instance().get_asset_<T>(id);
		}

		template<typename T>
		static int insert_asset(std::shared_ptr<T> &ptr) {
			return get_instance().insert_asset_<T>(ptr);
		}
	};

}


#endif // !ENG_ASSET_MANAGER_H_
