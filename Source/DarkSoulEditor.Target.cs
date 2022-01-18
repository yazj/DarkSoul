//  Copyright Yazj, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class DarkSoulEditorTarget : TargetRules
{
	public DarkSoulEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "DarkSoul" } );
	}
}
