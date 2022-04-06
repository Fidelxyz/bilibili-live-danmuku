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
    Copy-Item .\release\$targetName .\$archiveName\ # copy exe

    # copy dependency
    windeployqt --compiler-runtime .\$archiveName\$targetName
    Copy-Item .\3rdparty\bin\*.dll .\$archiveName\

    $excludeList = @("*.qmlc", "*.ilk", "*.exp", "*.lib", "*.pdb")
    Remove-Item -Path .\$archiveName -Include $excludeList -Recurse -Force

    Compress-Archive -Path .\$archiveName .\$archiveName'.zip'
}

if ($null -eq $archiveName -or $null -eq $targetName) {
    Write-Host "args missing, archiveName is" $archiveName ", targetName is" $targetName
    return
}
Main
