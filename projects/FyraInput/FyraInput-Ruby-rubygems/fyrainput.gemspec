Gem::Specification.new do |s|
  s.name = 'fyrainput'
  s.version = '1.0.0'
  s.summary = 'Direct Windows x64 FyraInput input SDK'
  s.description = 'Native bindings to the product SDK: receive, edit, send, descriptors, injection and cancellation. Matching driver installed separately.'
  s.authors = ['TitanCSA']
  s.license = 'MIT'
  s.homepage = 'https://github.com/TitanCSA/FyraInput-SDK'
  s.required_ruby_version = '>= 3.1'
  s.files = Dir['lib/**/*', 'native/*.dll', 'docs/**/*', 'examples/**/*', 'test/*.rb', 'README.md', 'LICENSE']
  s.require_paths = ['lib']
  s.add_runtime_dependency 'fiddle', '>= 1.1', '< 2'
  s.metadata = {'source_code_uri'=>s.homepage+'/tree/main/FyraInput-Ruby-rubygems', 'documentation_uri'=>s.homepage+'/tree/main/FyraInput-Ruby-rubygems/docs', 'allowed_push_host'=>'https://rubygems.org'}
end
