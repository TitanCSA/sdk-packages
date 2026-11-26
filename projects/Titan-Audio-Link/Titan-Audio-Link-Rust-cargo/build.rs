use std::{env, fs, path::PathBuf};
fn main() {
    let dir = env::var_os("TITAN_AUDIO_LINK_SDK_LIB_DIR")
        .map(PathBuf::from)
        .unwrap_or_else(|| {
            PathBuf::from(env::var_os("CARGO_MANIFEST_DIR").unwrap()).join("native")
        });
    println!("cargo:rustc-link-search=native={}", dir.display());
    println!("cargo:rustc-link-lib=dylib=TitanAudioLinkJsonSDK");
    println!("cargo:rerun-if-env-changed=TITAN_AUDIO_LINK_SDK_LIB_DIR");
    println!(
        "cargo:rerun-if-changed={}",
        dir.join("TitanAudioLinkJsonSDK.dll").display()
    );
    let out = PathBuf::from(env::var_os("OUT_DIR").expect("Cargo must set OUT_DIR"));
    let dll = "TitanAudioLinkJsonSDK.dll";
    // Cargo adds link-search paths under OUT_DIR to the loader path for run and test.
    fs::copy(dir.join(dll), out.join(dll))
        .unwrap_or_else(|error| panic!("Could not stage {dll}: {error}"));
    println!("cargo:rustc-link-search=native={}", out.display());
}
