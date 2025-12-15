param(
    [Parameter(Mandatory=$true)]
    [string]$text
)

$bytes = [System.Text.Encoding]::UTF8.GetBytes($text)
$b64 = [System.Convert]::ToBase64String($bytes)
Write-Output $b64