[CmdletBinding()]
param (
    [string] $archiveName, [string] $targetName
)

$scriptDir = $PSScriptRoot
$currentDir = Get-Location
Write-Host "currentDir" $currentDir
Write-Host "scriptDir" $scriptDir

function Main() {

    New-Item -ItemType Directory .\build\$archiveName # create directory
    Copy-Item .\build\release\$targetName .\build\$archiveName\ # copy exe

    # copy dependency
    windeployqt --compiler-runtime .\build\$archiveName\$targetName
    Copy-Item .\3rdparty\bin\*.dll .\build\$archiveName\

    $excludeList = @("*.qmlc", "*.ilk", "*.exp", "*.lib", "*.pdb")
    Remove-Item -Path .\build\$archiveName -Include $excludeList -Recurse -Force

    Compress-Archive -Path .\build\$archiveName .\build\$archiveName'.zip'
}

if ($null -eq $archiveName -or $null -eq $targetName) {
    Write-Host "args missing, archiveName is" $archiveName ", targetName is" $targetName
    return
}
Main
