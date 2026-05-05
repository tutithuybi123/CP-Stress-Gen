$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root ".tmp_build"
$ExamplesDir = Join-Path $Root "examples"

New-Item -ItemType Directory -Force $BuildDir | Out-Null

Push-Location $Root
try {
    Get-ChildItem -Path $ExamplesDir -Filter "*.cpp" | Sort-Object Name | ForEach-Object {
        $Name = [System.IO.Path]::GetFileNameWithoutExtension($_.Name)
        $Output = Join-Path $BuildDir "$Name.exe"
        Write-Host "[BUILD] examples/$($_.Name)"
        & g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude $_.FullName -o $Output
        if ($LASTEXITCODE -ne 0) {
            throw "Failed to build examples/$($_.Name)"
        }
    }

    Write-Host "[PASS] build_examples"
} finally {
    Pop-Location
}
