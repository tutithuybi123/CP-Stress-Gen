$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root ".tmp_build"
$TestsDir = Join-Path $Root "tests"

Get-ChildItem -Path $TestsDir -Filter "*.cpp" | Sort-Object Name | ForEach-Object {
    $Name = [System.IO.Path]::GetFileNameWithoutExtension($_.Name)
    $Executable = Join-Path $BuildDir "$Name.exe"
    if (!(Test-Path $Executable)) {
        throw "Missing test executable: $Executable"
    }
    Write-Host "[RUN] $Name"
    & $Executable
    if ($LASTEXITCODE -ne 0) {
        throw "Failed test: $Name"
    }
}

Write-Host "[PASS] run_tests"
