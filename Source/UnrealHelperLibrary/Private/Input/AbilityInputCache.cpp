// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AbilityInputCache.h"

#include "AbilitySystem/UHLAbilitySystemComponent.h"


DEFINE_LOG_CATEGORY(Log_UHL_AbilityInputCache);

void UAbilityInputCache::SetUp(UUHLAbilitySystemComponent* ASC_In)
{
    if (!ASC_In) return;

    ASC = ASC_In;
}

bool UAbilityInputCache::AddTagToCache(FGameplayTag AbilityTag_In)
{
    // not required to be unique
    AbilityInputCache.Add(AbilityTag_In);
    UE_LOG(Log_UHL_AbilityInputCache, Log, TEXT("Add ability to InputCache - %s"), *AbilityTag_In.ToString());
    return true;
}

bool UAbilityInputCache::AddTagsToCache(TArray<FGameplayTag> AbilityTags_In, bool bReverse)
{
    if (AbilityTags_In.IsEmpty()) return false;

    if (bReverse)
    {
        for (int32 i = AbilityTags_In.Num() - 1; i >= 0; i--)
        {
            AddTagToCache(AbilityTags_In[i]);
        }
    }
    else
    {
        for (int32 i = 0; i < AbilityTags_In.Num(); i++)
        {
            AddTagToCache(AbilityTags_In[i]);
        }
    }

    return true;
}

// should cancel all abilities that also cached but not fired
void UAbilityInputCache::CheckCache()
{
    UE_LOG(Log_UHL_AbilityInputCache, Log, TEXT("Check AbilityInputCache"));

    if (AbilityInputCache.IsEmpty())
    {
        UE_LOG(Log_UHL_AbilityInputCache, Log, TEXT("AbilityInputCache is empty"));
        return;
    };

    if (ASC.IsValid())
    {
        bool bActivatedAtLeastOneTag = false;
        for (int32 i = AbilityInputCache.Num() - 1; i >= 0; i--)
        {
            FGameplayTag AbilityTagToActivate = AbilityInputCache[i];
			if (AbilityTagToActivate.IsValid())
			{
				// 1) активировать некст абилку и уповать на то что rollattack не активируется потому что условия не подходят

				// 2) [средне хуйевый] сделать добавление в разные типы кэшей, тогда будет кэш Default/Normal/RollCache,
				// - Default - кэш содержащий все действия, если нет приоритезации cache'а, то из Default берется действие
				// - Normal - обычные атаки и действия(кувырок, прыжок)
				// - RollCache - roll атаки и действия(кувырок, прыжок)
				// в check cache
				// говорю приоритезируй RollCache, он берет и достает кувырочную атаку либо сам roll, либо чет еще что в RollCache'е есть
				// т.е. у каждой абилки есть несколько кешей в которые она заносится - Default/Normal/RollCache/JumpCache
				// мб тогда наоборот добавлять действие во все cache'ы, а указывать в абилке только те кэши
				// в которые ее добавлять не надо, ExcludeFromCaches TArray<FString> и список кэшей в которые абилку
				// включать не стоит, минусы - контр-интуитивно, плюсы - "типо меньше работы"

				// [хуйня варик] 3) prioritize abilities matching tag и тогда в кувырке я смогу сказать приоритезируй
				// атаку кувырочную, а не обычную удобненько, а как эта приоритезация должна работать, вот есть такой стэк
				// - Roll
				// - MeleeAttack.RightHand
				// - MeleeAttack.Roll
				// если говорю приоритезируй атаку MeleeAttack.Roll, то возьмет не Roll, а MeleeAttack.Roll - хуйня варик

				// 4) [вроде оч гибкий] CacheInputActivationRequiredTags/CacheInputBlockedTags - запретить если чел не в кувырке добавлять некоторые абилки в cache
				// пример есть Roll атака, если мы не в Roll'е тогда зачем ее добавлять в Cache воообще, нужно не только
				// bCacheInput чекать, но и вот эти теги присутствуют/отсутствуют.
				// Единственная проблема вот я кувырок и ЛКМ почти одновременно нажал че произойдет?
				// успеет ли tag кувырка наложится прежде чем я

				// TODO should we get another Ability from cache to activate
				// if failed to activation of last ability failed ??? mb it should be an option
				UE_LOG(Log_UHL_AbilityInputCache, Log, TEXT("InputCache checked found ability to activate - %s!"), *AbilityTagToActivate.ToString());
				bool bActivated = ASC->TryActivateAbilityWithTag(AbilityTagToActivate);
				UE_LOG(Log_UHL_AbilityInputCache, Log, TEXT("Activation %s - %s!"), bActivated ? TEXT("successful") : TEXT("failed"), *AbilityTagToActivate.ToString());

			    if (bActivated)
			    {
			        bActivatedAtLeastOneTag = true;
			        break;
			    }
			}
        }

        // possibly is shit, ANS_CheckAbilityInputCache clears cache on end (forgot how its related)
        if (bActivatedAtLeastOneTag)
        {
            ClearCache();
            UE_LOG(Log_UHL_AbilityInputCache, Log, TEXT("AbilityInputCache activated at least on tag from cache - clean"));
        }
    }
}

void UAbilityInputCache::ClearCache()
{
    AbilityInputCache.Empty();
}

// void UAbilityInputCache::RemoveTagFromCache(FGameplayTag AbilityTag_In)
// {
//     AbilityInputCache.Remove(AbilityTag_In);
// }
