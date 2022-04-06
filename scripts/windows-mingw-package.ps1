[CmdletBinding()]
param (
    [string] $archiveName, [string] $targetName
)

$scriptDir = $PSScriptRoot
$currentDir = Get-Location
Write-Host "currentDir" $currentDir
Write-Host "scriptDir" $scriptDir

function Main() {

    New-Item -ItemType Directory .\$archiveName # create directory
    Copy-Item .\build\release\$targetName .\$archiveName\ # copy exe
    windeployqt --compiler-runtime .\$archiveName\$targetName # copy dependency

    $excludeList = @("*.qmlc", "*.ilk", "*.exp", "*.lib", "*.pdb")
    Remove-Item -Path .\$archiveName -Include $excludeList -Recurse -Force

    Compress-Archive -Path .\$archiveName .\$archiveName'.zip'
}

if ($null -eq $archiveName -or $null -eq $targetName) {
    Write-Host "args missing, archiveName is" $archiveName ", targetName is" $targetName
    return
}
Main
