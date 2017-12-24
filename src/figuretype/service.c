#include "service.h"

#include "building/building.h"
#include "building/market.h"
#include "figure/image.h"
#include "figure/route.h"
#include "graphics/image.h"

#include "FigureAction.h"
#include "FigureMovement.h"
#include "Terrain.h"

static void roamer_action(figure *f, int num_ticks)
{
    switch (f->actionState) {
        case FigureActionState_150_Attack:
            FigureAction_Common_handleAttack(f);
            break;
        case FigureActionState_149_Corpse:
            FigureAction_Common_handleCorpse(f);
            break;
        case FIGURE_ACTION_125_ROAMING:
            f->isGhost = 0;
            f->roamLength++;
            if (f->roamLength >= f->maxRoamLength) {
                int x, y;
                building *b = building_get(f->buildingId);
                if (Terrain_getClosestRoadWithinRadius(b->x, b->y, b->size, 2, &x, &y)) {
                    f->actionState = FIGURE_ACTION_126_ROAMER_RETURNING;
                    f->destinationX = x;
                    f->destinationY = y;
                    figure_route_remove(f);
                    f->roamLength = 0;
                } else {
                    f->state = FigureState_Dead;
                }
            }
            FigureMovement_roamTicks(f, num_ticks);
            break;
        case FIGURE_ACTION_126_ROAMER_RETURNING:
            FigureMovement_walkTicks(f, num_ticks);
            if (f->direction == DIR_FIGURE_AT_DESTINATION ||
                f->direction == DIR_FIGURE_REROUTE || f->direction == DIR_FIGURE_LOST) {
                f->state = FigureState_Dead;
            }
            break;
    }
}

static void culture_action(figure *f, int group)
{
    f->terrainUsage = FigureTerrainUsage_Roads;
    f->useCrossCountry = 0;
    f->maxRoamLength = 384;
    building *b = building_get(f->buildingId);
    if (!BuildingIsInUse(b) || b->figureId != f->id) {
        f->state = FigureState_Dead;
    }
    figure_image_increase_offset(f, 12);
    roamer_action(f, 1);
    figure_image_update(f, image_group(group));
}

void figure_priest_action(figure *f)
{
    culture_action(f, GROUP_FIGURE_PRIEST);
}

void figure_school_child_action(figure *f)
{
    f->terrainUsage = FigureTerrainUsage_Roads;
    f->useCrossCountry = 0;
    f->maxRoamLength = 96;
    building *b = building_get(f->buildingId);
    if (!BuildingIsInUse(b) || b->type != BUILDING_SCHOOL) {
        f->state = FigureState_Dead;
    }
    figure_image_increase_offset(f, 12);
    switch (f->actionState) {
        case FigureActionState_150_Attack:
            FigureAction_Common_handleAttack(f);
            break;
        case FigureActionState_149_Corpse:
            FigureAction_Common_handleCorpse(f);
            break;
        case FIGURE_ACTION_125_ROAMING:
            f->isGhost = 0;
            f->roamLength++;
            if (f->roamLength >= f->maxRoamLength) {
                f->state = FigureState_Dead;
            }
            FigureMovement_roamTicks(f, 2);
            break;
    }
    figure_image_update(f, image_group(GROUP_FIGURE_SCHOOL_CHILD));
}

void figure_teacher_action(figure *f)
{
    culture_action(f, GROUP_FIGURE_TEACHER_LIBRARIAN);
}

void figure_librarian_action(figure *f)
{
    culture_action(f, GROUP_FIGURE_TEACHER_LIBRARIAN);
}

void figure_barber_action(figure *f)
{
    culture_action(f, GROUP_FIGURE_BARBER);
}

void figure_bathhouse_worker_action(figure *f)
{
    culture_action(f, GROUP_FIGURE_BATHHOUSE_WORKER);
}

void figure_doctor_action(figure *f)
{
    culture_action(f, GROUP_FIGURE_DOCTOR_SURGEON);
}

void figure_missionary_action(figure *f)
{
    f->terrainUsage = FigureTerrainUsage_Roads;
    f->useCrossCountry = 0;
    f->maxRoamLength = 192;
    building *b = building_get(f->buildingId);
    if (!BuildingIsInUse(b) || b->figureId != f->id) {
        f->state = FigureState_Dead;
    }
    figure_image_increase_offset(f, 12);
    roamer_action(f, 1);
    figure_image_update(f, image_group(GROUP_FIGURE_MISSIONARY));
}

void figure_patrician_action(figure *f)
{
    f->terrainUsage = FigureTerrainUsage_Roads;
    f->useCrossCountry = 0;
    f->maxRoamLength = 128;
    if (!BuildingIsInUse(building_get(f->buildingId))) {
        f->state = FigureState_Dead;
    }
    figure_image_increase_offset(f, 12);
    roamer_action(f, 1);
    figure_image_update(f, image_group(GROUP_FIGURE_PATRICIAN));
}

void figure_labor_seeker_action(figure *f)
{
    f->terrainUsage = FigureTerrainUsage_Roads;
    f->useCrossCountry = 0;
    f->maxRoamLength = 384;
    building *b = building_get(f->buildingId);
    if (!BuildingIsInUse(b) || b->figureId2 != f->id) {
        f->state = FigureState_Dead;
    }
    figure_image_increase_offset(f, 12);
    roamer_action(f, 1);
    figure_image_update(f, image_group(GROUP_FIGURE_LABOR_SEEKER));
}

void figure_market_trader_action(figure *f)
{
    f->terrainUsage = FigureTerrainUsage_Roads;
    f->useCrossCountry = 0;
    f->maxRoamLength = 384;
    building *market = building_get(f->buildingId);
    if (!BuildingIsInUse(market) || market->figureId != f->id) {
        f->state = FigureState_Dead;
    }
    figure_image_increase_offset(f, 12);
    if (f->actionState == FIGURE_ACTION_125_ROAMING) {
        // force return on out of stock
        int stock = building_market_get_max_food_stock(market) +
            building_market_get_max_goods_stock(market);
        if (f->roamLength >= 96 && stock <= 0) {
            f->roamLength = f->maxRoamLength;
        }
    }
    roamer_action(f, 1);
    figure_image_update(f, image_group(GROUP_FIGURE_MARKET_LADY));
}

