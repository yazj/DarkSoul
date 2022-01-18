//  Copyright Yazj, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class DarkSoulTarget : TargetRules
{
	public DarkSoulTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "DarkSoul" } );
	}
}
