param(
  [string]$Configuration,
  [string]$OutputUnityAssetsDirectory,
  [string]$EOS_SDK_DLL_NAME,
  [string]$STEAM_API_DLL_NAME,
  [string]$TargetDir,
  [string]$SteamLibraryDirectory,
  [string]$SolutionDir
)

Write-Host "=============== Start of Prebuild Script =========================="

# If in debug mode, we want to copy the eos sdk dll, the steam app id file, and
# The steam DLL into the output directory.
# Note: STEAM_API_DLL_NAME may be empty (e.g. ARM64, where Steam has no native binary).
# In that case the Steam app id file and Steam DLL are skipped.
if ($Configuration -eq "Debug")
{
  $filesToCopy = @(
    (Join-Path $OutputUnityAssetsDirectory $EOS_SDK_DLL_NAME)
  )

  if (-not [string]::IsNullOrWhiteSpace($STEAM_API_DLL_NAME))
  {
    $filesToCopy += (Join-Path $SolutionDir "..\..\..\steam_appid.txt")
    $filesToCopy += (Join-Path $SteamLibraryDirectory $STEAM_API_DLL_NAME)
  }

  $filesToCopy | ForEach-Object {
    if (-Not(Test-Path -LiteralPath $_)) {
      Write-Host ("WARNING: Could not copy file '{0}' because it does not exist. This might not be a problem." -f $_);
      continue
    }

    $itemBeingCopied = Get-Item $_;
    Write-Host ('Copying file "{0}" to "{1}".' -f $itemBeingCopied.FullName, $TargetDir);
    Copy-Item -LiteralPath $itemBeingCopied.FullName -Destination $TargetDir -Force;
  }
}

Write-Host "=============== End of Prebuild Script =========================="