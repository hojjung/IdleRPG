// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
    public class VICODynamicsPlugin : ModuleRules
	{
        static readonly bool UseParallelizedBuild = false;

		public VICODynamicsPlugin(ReadOnlyTargetRules Target) : base(Target)
		{
            string VICODynamicsDir = ModuleDirectory + "/VICODynamics/";

            PublicIncludePaths.AddRange(
				new string[] 
                {
                    ModuleDirectory + "/Public",
                    VICODynamicsDir + "Include"
                });
            
            PrivateIncludePaths.AddRange(
				new string[] 
                {
                    ModuleDirectory + "/Private"
                });

		    PublicDependencyModuleNames.AddRange(
		        new string[]
		        {
		            "Core",
		            "CoreUObject",
                    "Engine",
		            "RenderCore",
//		            "ShaderCore",
		            "RHI",
                    "PhysX"
		        });

            if (Target.bBuildEditor)
                PublicDependencyModuleNames.Add("UnrealEd");

            PrivateDependencyModuleNames.AddRange(
		        new string[]
		        {
		            "Engine",
                    "PhysX"
                });

            PrivateIncludePathModuleNames.Add("Engine");
            
            if (Target.Platform != UnrealTargetPlatform.Android && Target.Platform != UnrealTargetPlatform.HTML5) // Not supported on Android and HTML5
            {
                PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "APEX"
                });

                PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "APEX"
                });
            }

		    SetupVICODynamicsLib(Target);

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        }

        private void SetupVICODynamicsLib(ReadOnlyTargetRules Target)
        {
            string Config = "Profile";

            switch (Target.Configuration)
            {
                case UnrealTargetConfiguration.Debug:
                    if (Target.bDebugBuildsActuallyUseDebugCRT)
                    {
                        Config = "Debug";
                        PublicDefinitions.Add("VICODYNAMICS_DEBUG");
                    }
                    else
                        Config = "Checked";
                    break;
                case UnrealTargetConfiguration.Development:
                case UnrealTargetConfiguration.DebugGame:
                case UnrealTargetConfiguration.Unknown:
                    Config = "Profile";
                    break;
                case UnrealTargetConfiguration.Test:
                case UnrealTargetConfiguration.Shipping:
                    Config = "Release";
                    break;
            }
            
            if (UseParallelizedBuild)
            {
                Config += "_Parallel";
                PublicDefinitions.Add("VICODYNAMICS_USE_PARALLEL=1");
            }
            else
            {
                PublicDefinitions.Add("VICODYNAMICS_USE_PARALLEL=0");
            }

            //Determine the root directory
            string VICODynamicsDir = ModuleDirectory + "/VICODynamics/";

            string VICODynamicsLibDir = VICODynamicsDir + "Lib/";
            
            // Libraries and DLLs for windows platform
            if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
            {
                VICODynamicsLibDir += (Target.Platform == UnrealTargetPlatform.Win64 ? "Win64/" : "Win32/") + Config + "/";
                PublicLibraryPaths.Add(VICODynamicsLibDir);
                PublicAdditionalLibraries.Add(String.Format("VICODynamics_{0}.lib", Config));
                PublicDelayLoadDLLs.Add(String.Format("VICODynamics_{0}.dll", Config));
                RuntimeDependencies.Add(VICODynamicsLibDir + String.Format("VICODynamics_{0}.dll", Config));
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                VICODynamicsLibDir += "OSX64/" + Config + "/";

                PublicAdditionalLibraries.Add(String.Format("{0}libVICODynamics.a", VICODynamicsLibDir));

                //PublicDelayLoadDLLs.Add(String.Format("{0}libVICODynamics.dylib", VICODynamicsLibDir));
                //RuntimeDependencies.Add(VICODynamicsLibDir + "libVICODynamics.dylib");
            }
            else if (Target.Platform == UnrealTargetPlatform.Android)
            {

            }
            else if (Target.Platform == UnrealTargetPlatform.Linux)
            {

            }
            else if (Target.Platform == UnrealTargetPlatform.IOS)
            {

            }
            else if (Target.Platform == UnrealTargetPlatform.PS4)
            {
                VICODynamicsLibDir += "PS4/" + Config + "/";
                PublicLibraryPaths.Add(VICODynamicsLibDir);
                PublicAdditionalLibraries.Add(String.Format("VICODynamics_{0}_stub_weak", Config));
                PublicDelayLoadDLLs.Add(String.Format("libVICODynamics_{0}.prx", Config));
                RuntimeDependencies.Add(VICODynamicsLibDir + String.Format("libVICODynamics_{0}.prx", Config));
            }
            else if (Target.Platform == UnrealTargetPlatform.XboxOne)
            {
                VICODynamicsLibDir += "XboxOne/" + Config + "/";
                PublicLibraryPaths.Add(VICODynamicsLibDir);
                PublicAdditionalLibraries.Add(String.Format("VICODynamics_{0}.lib", Config));
                //PublicAdditionalLibraries.Add("delayimp.lib"); // Required for __delayLoadHelper2 reference when using DelayLoaded DLLs [delayimp.lib is a Microsoft lib]
                //PublicDelayLoadDLLs.Add(String.Format("VICODynamics_{0}.dll", Config));
                //var PathC = CopyToProjectBinaries(VICODynamicsLibDir + String.Format("VICODynamics_{0}.dll", Config), Target);
                //System.Console.WriteLine(PathC);
                RuntimeDependencies.Add(VICODynamicsLibDir + String.Format("VICODynamics_{0}.dll", Config));
            }
            else if (Target.Platform == UnrealTargetPlatform.Switch)
            {
                VICODynamicsLibDir += "Switch/" + Config + "/";
                PublicLibraryPaths.Add(VICODynamicsLibDir);
                PublicAdditionalLibraries.Add(VICODynamicsLibDir + String.Format("VICODynamics_{0}.a", Config));
                //PublicDelayLoadDLLs.Add(VICODynamicsLibDir + String.Format("VICODynamics_{0}.nro", Config));
                //RuntimeDependencies.Add(VICODynamicsLibDir + String.Format("VICODynamics_{0}.nro", Config));
            }
        }

        public string GetUProjectPath()
        {
            return Path.Combine(ModuleDirectory, "../../../../../AAWX");
        }

        private string CopyToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
        {
            string BinariesDir = Path.Combine(GetUProjectPath(), "Binaries", Target.Platform.ToString());
            string Filename = Path.GetFileName(Filepath);

            //convert relative path 
            string FullBinariesDir = Path.GetFullPath(BinariesDir);

            if (!Directory.Exists(FullBinariesDir))
            {
                Directory.CreateDirectory(FullBinariesDir);
            }

            string FullExistingPath = Path.Combine(FullBinariesDir, Filename);
            bool ValidFile = false;

            //File exists, check if they're the same
//             if (File.Exists(FullExistingPath))
//             {
//                 ValidFile = true;
//             }

            //No valid existing file found, copy new dll
            //if (!ValidFile)
            {
                File.Copy(Filepath, Path.Combine(FullBinariesDir, Filename), true);
            }
            return FullExistingPath;
        }
    }
}
