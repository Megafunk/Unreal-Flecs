﻿
#if WITH_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Fixtures/FlecsWorldFixture.h"
#include "Transforms/FlecsTransformDefaultEntities.h"

BEGIN_DEFINE_SPEC(FDefaultEntityTestsSpec,
                  "UnrealFlecs.Entity.DefaultEntity",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

FFlecsTestFixture Fixture;

END_DEFINE_SPEC(FDefaultEntityTestsSpec);

DEFINE_DEFAULT_ENTITY_OPTION(TestEntityOption);

void FDefaultEntityTestsSpec::Define()
{
	FLECS_FIXTURE_LIFECYCLE(Fixture);

	Describe("Default Entity Validity", [&]()
	{
		It("Should be valid from the same module", [&]()
		{
			TestTrue("Default entity should be valid",
				Fixture.FlecsWorld->GetEntity(TestEntityOption).IsValid());
		});
		
		It("Should be valid from another module", [&]()
		{
			TestTrue("Default entity should be valid",
				Fixture.FlecsWorld->GetEntity(FlecsLocalTrait).IsValid());
		});
	});
}

#endif // WITH_AUTOMATION_TESTS
