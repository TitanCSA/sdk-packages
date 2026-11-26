Gem::Specification.new do |s|
  s.name='vdevice'
  s.version = '1.0.1'
  s.summary='Windows x64 Vdevice direct SDK'
  s.description='Product native SDK operations through a UTF-8 JSON C ABI; driver installed separately.'
  s.authors=['TitanCSA']
  s.license='MIT'
  s.homepage='https://github.com/TitanCSA/Vdevice'
  s.required_ruby_version='>=3.1'
  s.files=Dir['lib/**/*','native/*','docs/*','README.md','LICENSE']
  s.require_paths=['lib']
  s.add_runtime_dependency 'fiddle','>=1.1','<2'
  s.metadata = {
    'source_code_uri' => 'https://github.com/TitanCSA/sdk-packages/tree/main/projects/Vdevice/Vdevice-Ruby-rubygems',
    'documentation_uri' => 'https://github.com/TitanCSA/sdk-packages/tree/main/projects/Vdevice/Vdevice-Ruby-rubygems#readme',
    'allowed_push_host' => 'https://rubygems.org'
  }
end
