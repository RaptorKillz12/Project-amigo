# Authorized compatibility research

This document is a safe starting point for investigating cross-platform compatibility. It does not describe extracting or modifying protected commercial game packages.

## Scope

Use source code, builds, and assets that you created or are explicitly authorized to port. Do not bypass platform protections or redistribute copyrighted game files.

## Prototype plan

- Create a small Unity test project with a scene, sound, and configurable input.
- Build it from source for Windows and Android using the corresponding Unity modules.
- Add platform adapters for input and lifecycle behavior, then verify each target independently.
- Use logs and profiling to diagnose compatibility issues.
- Only evaluate a game-specific porting framework against its documented requirements and an authorized build.

## Limitations

A blank project shell or a native stub library is not a translation layer. A real compatibility solution must implement the specific runtime and platform interfaces the application depends on. Compatibility, performance, and legal authorization must be assessed for each title and target.
