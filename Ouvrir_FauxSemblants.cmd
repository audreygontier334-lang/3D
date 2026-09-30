@echo off
setlocal EnableExtensions
chcp 65001 >nul
title Faux-semblants - mise a jour et lancement

set "REPO=%USERPROFILE%\Documents\GitHub\3D"
if exist "%REPO%\.git" goto repo_ok

echo Dossier habituel introuvable :
echo %REPO%
echo.
set /p "REPO=Colle ici le chemin du dossier 3D, puis appuie sur Entree : "
set "REPO=%REPO:"=%"

:repo_ok
if not exist "%REPO%\.git" (
  echo.
  echo Ce dossier ne contient pas le depot GitHub 3D.
  echo Relance ce fichier et indique le dossier clone par GitHub Desktop.
  pause
  exit /b 1
)

tasklist /FI "IMAGENAME eq UnrealEditor.exe" 2>nul | find /I "UnrealEditor.exe" >nul
if not errorlevel 1 (
  echo.
  echo Ferme Unreal Engine, puis relance ce fichier.
  pause
  exit /b 1
)

set "GIT=git"
where git >nul 2>nul
if not errorlevel 1 goto git_ok

set "GIT="
if exist "%ProgramFiles%\Git\cmd\git.exe" set "GIT=%ProgramFiles%\Git\cmd\git.exe"
if defined GIT goto git_ok

for /f "delims=" %%G in ('dir /b /s "%LOCALAPPDATA%\GitHubDesktop\git.exe" 2^>nul') do if not defined GIT set "GIT=%%G"
if defined GIT goto git_ok

echo.
echo Git est introuvable. Ouvre GitHub Desktop une fois, puis relance ce fichier.
pause
exit /b 1

:git_ok
pushd "%REPO%"
if errorlevel 1 goto failed

set "DIRTY="
for /f "delims=" %%I in ('"%GIT%" status --porcelain') do set "DIRTY=1"
if defined DIRTY (
  echo.
  echo Des fichiers locaux ont ete modifies. Rien n'a ete ecrase.
  echo Ouvre GitHub Desktop pour conserver ou annuler ces changements.
  popd
  pause
  exit /b 1
)

echo.
echo Recuperation de la version principale...
"%GIT%" fetch origin
if errorlevel 1 goto failed_popd

"%GIT%" show-ref --verify --quiet refs/heads/main
if errorlevel 1 (
  "%GIT%" switch --track -c main origin/main
) else (
  "%GIT%" switch main
)
if errorlevel 1 goto failed_popd

"%GIT%" pull --ff-only origin main
if errorlevel 1 goto failed_popd

set "PROJECT=%REPO%\Game\Unreal\FauxSemblants\FauxSemblants.uproject"
if not exist "%PROJECT%" goto missing_project

echo.
echo Mise a jour terminee. Ouverture du projet Unreal...
start "" "%PROJECT%"
popd
exit /b 0

:missing_project
echo.
echo Projet Unreal introuvable : %PROJECT%
popd
pause
exit /b 1

:failed_popd
popd
:failed
echo.
echo La mise a jour a echoue. Copie le message affiche et envoie-le a Codex.
pause
exit /b 1
