Gem::Specification.new do |s|
  s.name = 'interceptor-multilang'
  s.version = '1.0.0'
  s.summary = 'Direct Windows x64 Interceptor input SDK for Ruby'
  s.description = 'Fiddle bindings with native DLL, API docs and examples. The matching input driver is installed separately.'
  s.authors = ['TitanCSA']
  s.license = 'MIT'
  s.homepage = 'https://github.com/TitanCSA/Interceptor-MultiLang'
  s.required_ruby_version = '>= 3.1'
  s.files = Dir['lib/**/*', 'native/InterceptorBridge.dll', 'docs/**/*', 'examples/**/*', 'test/**/*.rb', 'README.md', 'LICENSE', 'CHANGELOG.md']
  s.require_paths = ['lib']
  s.add_runtime_dependency 'fiddle', '>= 1.1', '< 2.0'
  s.metadata = {'source_code_uri' => s.homepage, 'documentation_uri' => s.homepage + '/blob/main/docs/API.md', 'bug_tracker_uri' => s.homepage + '/issues', 'allowed_push_host' => 'https://rubygems.org'}
end
