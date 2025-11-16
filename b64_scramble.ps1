param(
    [Parameter(Mandatory=$true)]
    [string]$text
)

$bytes = [System.Text.Encoding]::UTF8.GetBytes($text)
$base64 = [System.Convert]::ToBase64String($bytes)
Write-Output $base64