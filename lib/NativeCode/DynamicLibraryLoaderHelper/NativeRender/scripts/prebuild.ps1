param(
  [string]$Configuration,
  [string]$NormalizedPlatform,
  [string]$OutputUnityAssetsDirectory,
  [string]$EOS_SDK_DLL_NAME,
  [string]$STEAM_API_DLL_NAME,
  [string]$TargetDir,
  [string]$SteamLibraryDirectory,
  [string]$SolutionDir
)

Write-Host "=============== Start of Prebuild Script =========================="

Write-Host "Configuration: $Configuration"
Write-Host "NormalizedPlatform: $NormalizedPlatform"
Write-Host "OutputUnityAssetsDirectory: $OutputUnityAssetsDirectory"
Write-Host "EOS_SDK_DLL_NAME: $EOS_SDK_DLL_NAME"
Write-Host "STEAM_API_DLL_NAME: $STEAM_API_DLL_NAME"
Write-Host "TargetDir: $TargetDir"
Write-Host "SteamLibraryDirectory: $SteamLibraryDirectory"
Write-Host "SolutionDir: $SolutionDir"

# If in debug mode, we want to copy the eos sdk dll, the steam app id file, and
# The steam DLL into the output directory
if ($Configuration -eq "Debug")
{
  $filesToCopy = @(
    (Join-Path $SolutionDir "..\..\..\steam_appid.txt"),
    (Join-Path $SteamLibraryDirectory $STEAM_API_DLL_NAME),
    (Join-Path $OutputUnityAssetsDirectory $EOS_SDK_DLL_NAME)
  )

  $filesToCopy | ForEach-Object {
    $itemBeingCopied = Get-Item $_;

    Write-Host ('Copying file "{0}" to "{1}".' -f $itemBeingCopied.FullName, $TargetDir);

    Copy-Item -LiteralPath $itemBeingCopied.FullName -Destination $TargetDir;
  }
}

Write-Host "=============== End of Prebuild Script =========================="