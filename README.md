# Project Amigo

A research scaffold for learning about cross-platform compatibility with software you own or are authorized to port.

This repository does not include *Samba de Amigo: Party-To-Go*, its assets, protected app files, or DRM-bypass tooling. It is not currently a playable port.

## Current repository status

The project currently contains GitHub Actions workflow templates. It does not contain a Unity project, a compatibility runtime, or an authorized game source build. A workflow that builds a blank Unity project cannot turn an iOS app into an Android or PC game.

## Safe next steps

1. Start with a small Unity application you created or have permission to port.
2. Record the source project's engine/toolchain version and target platform requirements.
3. Build and test separate Windows and Android targets from authorized source.
4. Add platform-specific input, graphics, audio, and lifecycle adapters incrementally.
5. Keep copyrighted game binaries and assets out of this repository unless you have explicit permission to include them.

A Project Dream-style technique is game- and build-specific; it is not a generic wrapper that converts any iOS app into a PC or Android app. Whether it can apply to a particular title depends on a compatible, authorized build and the porting framework's own requirements.
