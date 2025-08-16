#include "ecs/component.hpp"
#include "entt/entity/fwd.hpp"

namespace ecs {
void riversInit(entt::registry &registry) {
    const float spawnTime = 600.f;

    const entt::entity iceCrystalOriginEntity = registry.create();
    registry.emplace<Position>(iceCrystalOriginEntity, 869, -730);
    SP<Animation> iceCrystalAnimation = std::make_shared<Animation>("ice_crystal.aseprite");
    registry.emplace<CrystalOrigin>(iceCrystalOriginEntity, ecs::CrystalType::ice, 0.f, 1.f, iceCrystalAnimation, spawnTime, 6000.f);

    const entt::entity natureCrystalOriginEntity = registry.create();
    registry.emplace<Position>(natureCrystalOriginEntity, -1294, 117);
    SP<Animation> natureCrystalAnimation = std::make_shared<Animation>("nature_crystal.aseprite");
    registry.emplace<CrystalOrigin>(natureCrystalOriginEntity, ecs::CrystalType::nature, 1.f, 0.f, natureCrystalAnimation, spawnTime, 6000.f);

    const entt::entity lightningCrystalOriginEntity = registry.create();
    registry.emplace<Position>(lightningCrystalOriginEntity, -2, 727);
    SP<Animation> lightningCrystalAnimation = std::make_shared<Animation>("lightning_crystal.aseprite");
    registry.emplace<CrystalOrigin>(lightningCrystalOriginEntity, ecs::CrystalType::lightning, 0.f, -1.f, lightningCrystalAnimation, spawnTime, 2400.f);

    const entt::entity waterCrystalOriginEntity = registry.create();
    registry.emplace<Position>(waterCrystalOriginEntity, 1295, 449);
    SP<Animation> waterCrystalAnimation = std::make_shared<Animation>("water_crystal.aseprite");
    registry.emplace<CrystalOrigin>(waterCrystalOriginEntity, ecs::CrystalType::water, -1.f, 0.f, waterCrystalAnimation, spawnTime, 6500.f);

    const entt::entity fireCrystalOriginEntity = registry.create();
    registry.emplace<Position>(fireCrystalOriginEntity, -1294, -456);
    SP<Animation> fireCrystalAnimation = std::make_shared<Animation>("fire_crystal.aseprite");
    registry.emplace<CrystalOrigin>(fireCrystalOriginEntity, ecs::CrystalType::fire, 1.f, 0.f, fireCrystalAnimation, spawnTime, 6000.f);

	// Dragoons
	SP<Animation> fireDragoonAnimation = std::make_shared<Animation>("dragoons/firedragoon.aseprite");
	SP<Animation> waterDragoonAnimation = std::make_shared<Animation>("dragoons/waterdragoon.aseprite");
	SP<Animation> iceDragoonAnimation = std::make_shared<Animation>("dragoons/icedragoon.aseprite");
	SP<Animation> natureDragoonAnimation = std::make_shared<Animation>("dragoons/naturedragoon.aseprite");
	SP<Animation> lightningDragoonAnimation = std::make_shared<Animation>("dragoons/lightningdragoon.aseprite");
	const float dragoon_collider_radius = 25;

	const entt::entity fire = registry.create();
	registry.emplace<ecs::Position>(fire, -200, -600, 1);
	registry.emplace<ecs::Dragoon>(fire, ecs::CrystalType::fire);
	registry.emplace<ecs::CircleCollider>(fire, 0, 0, dragoon_collider_radius);
	registry.emplace<ecs::Sprite>(fire, fireDragoonAnimation, .6f);
	const entt::entity water = registry.create();
	registry.emplace<ecs::Position>(water, -100, -600, 1);
	registry.emplace<ecs::Dragoon>(water, ecs::CrystalType::water);
	registry.emplace<ecs::CircleCollider>(water, 0, 0, dragoon_collider_radius);
	registry.emplace<ecs::Sprite>(water, waterDragoonAnimation, .6f);
	const entt::entity ice = registry.create();
	registry.emplace<ecs::Position>(ice, 0, -600, 1);
	registry.emplace<ecs::Dragoon>(ice, ecs::CrystalType::ice);
	registry.emplace<ecs::CircleCollider>(ice, 0, 0, dragoon_collider_radius);
	registry.emplace<ecs::Sprite>(ice, iceDragoonAnimation, .6f);
	const entt::entity nature = registry.create();
	registry.emplace<ecs::Position>(nature, 100, -600, 1);
	registry.emplace<ecs::Dragoon>(nature, ecs::CrystalType::nature);
	registry.emplace<ecs::CircleCollider>(nature, 0, 0, dragoon_collider_radius);
	registry.emplace<ecs::Sprite>(nature, natureDragoonAnimation, .6f);
	const entt::entity lightning = registry.create();
	registry.emplace<ecs::Position>(lightning, 200, -600, 1);
	registry.emplace<ecs::Dragoon>(lightning, ecs::CrystalType::lightning);
	registry.emplace<ecs::CircleCollider>(lightning, 0, 0, dragoon_collider_radius);
	registry.emplace<ecs::Sprite>(lightning, lightningDragoonAnimation, .6f);
	// </>

    // Direction changers
    entt::entity dcf_1_1 = registry.create();
    registry.emplace<Position>(dcf_1_1, -475.f, -458.f);
    registry.emplace<CrystalDirectionChanger>(dcf_1_1, 0.f, 1.f);
    entt::entity dcf_2_1 = registry.create();
    registry.emplace<Position>(dcf_2_1, -475.f, -150.f);
    registry.emplace<CrystalDirectionChanger>(dcf_2_1, 1.f, 0.f);

	entt::entity dcn_1_1 = registry.create();
    registry.emplace<Position>(dcn_1_1, -1198.f, 117.f);
    registry.emplace<CrystalDirectionChanger>(dcn_1_1, 0.f, 1.f);
	entt::entity dcn_2_1 = registry.create();
    registry.emplace<Position>(dcn_2_1, -1198.f, 242.f);
    registry.emplace<CrystalDirectionChanger>(dcn_2_1, 1.f, 0.f);
	entt::entity dcn_3_1 = registry.create();
    registry.emplace<Position>(dcn_3_1, -362.f, 242.f);
    registry.emplace<CrystalDirectionChanger>(dcn_3_1, 0.f, -1.f);
	entt::entity dcn_4_1 = registry.create();
    registry.emplace<Position>(dcn_4_1, -362.f, 71.f);
    registry.emplace<CrystalDirectionChanger>(dcn_4_1, 1.f, 0.f);

	entt::entity dcw_1_1 = registry.create();
	registry.emplace<Position>(dcw_1_1, 331, 451);
	registry.emplace<CrystalDirectionChanger>(dcw_1_1, 0.f, -1.f);
	entt::entity dcw_2_1 = registry.create();
	registry.emplace<Position>(dcw_2_1, 331, 68);
	registry.emplace<CrystalDirectionChanger>(dcw_2_1, -1.f, 0.f);

	entt::entity dci_1_1 = registry.create();
	registry.emplace<Position>(dci_1_1, 869, -522);
	registry.emplace<CrystalDirectionChanger>(dci_1_1, 1.f, 0.f);
	entt::entity dci_2_1 = registry.create();
	registry.emplace<Position>(dci_2_1, 1008, -522);
	registry.emplace<CrystalDirectionChanger>(dci_2_1, 0.f, 1.f);
	entt::entity dci_3_1 = registry.create();
	registry.emplace<Position>(dci_3_1, 1008, -153);
	registry.emplace<CrystalDirectionChanger>(dci_3_1, -1.f, 0.f);
    // </>

	// Crystal murderers
	entt::entity cml = registry.create();
	registry.emplace<Position>(cml, -1, 187);
	registry.emplace<CrystalMurderer>(cml);
	registry.emplace<Crystal>(cml, CrystalType::lightning);
	registry.emplace<Explosion>(cml, 2.f, [&registry](entt::entity culprit) {
		return registry.any_of<Crystal>(culprit);
	});
	registry.emplace<CircleCollider>(cml, 0, 0, 10);
	entt::entity cmf_1 = registry.create();
	registry.emplace<Position>(cmf_1, -230, -149);
	registry.emplace<CrystalMurderer>(cmf_1);
	registry.emplace<Crystal>(cmf_1, CrystalType::fire);
	registry.emplace<Explosion>(cmf_1, 2.f, [&registry](entt::entity culprit) {
		return registry.any_of<Crystal>(culprit);
	});
	registry.emplace<CircleCollider>(cmf_1, 0, 0, 10);
	entt::entity cmn_1 = registry.create();
	registry.emplace<Position>(cmn_1, -234, 69);
	registry.emplace<CrystalMurderer>(cmn_1);
	registry.emplace<Crystal>(cmn_1, CrystalType::nature);
	registry.emplace<Explosion>(cmn_1, 2.f, [&registry](entt::entity culprit) {
		return registry.any_of<Crystal>(culprit);
	});
	registry.emplace<CircleCollider>(cmn_1, 0, 0, 10);
	entt::entity cmw_1 = registry.create();
	registry.emplace<Position>(cmw_1, 257, 63);
	registry.emplace<CrystalMurderer>(cmw_1);
	registry.emplace<Crystal>(cmw_1, CrystalType::water);
	registry.emplace<Explosion>(cmw_1, 2.f, [&registry](entt::entity culprit) {
		return registry.any_of<Crystal>(culprit);
	});
	registry.emplace<CircleCollider>(cmw_1, 0, 0, 10);
	entt::entity cmi_1 = registry.create();
	registry.emplace<Position>(cmi_1, 249, -156);
	registry.emplace<CrystalMurderer>(cmi_1);
	registry.emplace<Crystal>(cmi_1, CrystalType::ice);
	registry.emplace<Explosion>(cmi_1, 2.f, [&registry](entt::entity culprit) {
		return registry.any_of<Crystal>(culprit);
	});
	registry.emplace<CircleCollider>(cmi_1, 0, 0, 10);
	// </>

	// RIVERS INIT
	SP<Animation> lightningRiver_Anim = std::make_shared<Animation>("rivers/LightningRiver.gif");

	SP<Animation> waterRiver_1 = std::make_shared<Animation>("rivers/Normal/WaterRiver-1.gif");
	SP<Animation> iceRiver_1 = std::make_shared<Animation>("rivers/Normal/IceRiver-1.gif");
	SP<Animation> fireRiver_1 = std::make_shared<Animation>("rivers/Normal/FireRiver-1.gif");
	SP<Animation> natureRiver_1 = std::make_shared<Animation>("rivers/Normal/NatureRiver-1.gif");

	SP<Animation> waterRiver_2 = std::make_shared<Animation>("rivers/Diverged/WaterRiver-2.gif");
	SP<Animation> iceRiver_2 = std::make_shared<Animation>("rivers/Diverged/IceRiver-2.gif");
	SP<Animation> fireRiver_2 = std::make_shared<Animation>("rivers/Diverged/FireRiver-2.gif");
	SP<Animation> natureRiver_2 = std::make_shared<Animation>("rivers/Diverged/NatureRiver-2.gif");

	entt::entity waterRiver = registry.create();
	registry.emplace<Position>(waterRiver, 0, 0, -1);
	registry.emplace<Sprite>(waterRiver, waterRiver_1, .6f);
	registry.emplace<River>(waterRiver, CrystalType::water, waterRiver_1, waterRiver_2);
	entt::entity iceRiver = registry.create();
	registry.emplace<Position>(iceRiver, 0, 0, -1);
	registry.emplace<Sprite>(iceRiver, iceRiver_1, .6f);
	registry.emplace<River>(iceRiver, CrystalType::ice, iceRiver_1, iceRiver_2);
	entt::entity fireRiver = registry.create();
	registry.emplace<Position>(fireRiver, 0, 0, -1);
	registry.emplace<Sprite>(fireRiver, fireRiver_1, .6f);
	registry.emplace<River>(fireRiver, CrystalType::fire, fireRiver_1, fireRiver_2);
	entt::entity natureRiver = registry.create();
	registry.emplace<Position>(natureRiver, 0, 0, -1);
	registry.emplace<Sprite>(natureRiver, natureRiver_1, .6f);
	registry.emplace<River>(natureRiver, CrystalType::nature, natureRiver_1, natureRiver_2);

	entt::entity lightningRiver = registry.create();
	registry.emplace<Position>(lightningRiver, 0, 0, -1);
	registry.emplace<Sprite>(lightningRiver, lightningRiver_Anim, .6f);
	registry.emplace<River>(lightningRiver, CrystalType::lightning, lightningRiver_Anim, lightningRiver_Anim);
	// </>

	entt::entity rest = registry.create();
	registry.emplace<Position>(rest, 0, 0);
	//registry.emplace<Sprite>(rest, std::make_shared<Animation>("rivers/Pentagon.aseprite"), .6f);
}
} // namespace ecs
