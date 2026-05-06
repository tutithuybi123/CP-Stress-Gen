$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
Push-Location $Root
try {
    & (Join-Path $PSScriptRoot "build_tests.ps1")
    & (Join-Path $PSScriptRoot "run_tests.ps1")
    & (Join-Path $PSScriptRoot "bundle_single_header.ps1")
    & (Join-Path $PSScriptRoot "build_examples.ps1")
    Write-Host "[PASS] check_all"
} finally {
    Pop-Location
}
