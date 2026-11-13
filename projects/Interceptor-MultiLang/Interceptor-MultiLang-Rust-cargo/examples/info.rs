use interceptor::Interceptor;
fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut io = Interceptor::new()?;
    io.close()?;
    Ok(())
}
