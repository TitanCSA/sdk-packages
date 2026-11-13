use interceptor::{Interceptor, Kind};
fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("Invert mouse Y. Close the program to restore normal input.");
    let mut io = Interceptor::new()?;
    loop {
        let mut inputs = io.receive(None)?;
        for input in &mut inputs.inputs {
            if input.kind == Kind::Mouse && input.state.scalars["flags"] & 1 == 0 {
                *input.state.scalars.get_mut("y").unwrap() *= -1;
            }
        }
        io.send(&mut inputs)?;
    }
}
