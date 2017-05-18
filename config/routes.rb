Rails.application.routes.draw do

  resources :parkingtotals
  resources :parkingslots
  root 'home#show'

  get 'test/index'

  resources :advertisements
  get 'dashboard/show'

  get 'ps6/index'


  resources :cameras
  get 'ps5/index'

  resources :configurations
  resources :zones
  resources :sites
  mount LetsencryptPlugin::Engine, at: '/'

  get 'ps4/index'

  get 'users/status'
  get 'parkinginfo/totalfree'
  get 'parkinginfo/free'
  post 'parkinginfo/addslot'
  get 'users/change'
  get 'users/addusertosite'
  get 'users/sitepermission'
  get 'help/about'
  get 'selector/change'
  get 'selector/select'

  get 'dashboard/test'
  post 'homography/calchm'
  get 'homography/calchm'

  devise_for :users
  get 'camcontrol/start'
  get 'camcontrol/stop'
  get 'camcontrol/generateimage'

  post 'dashboard/getInfo'

  # The priority is based upon order of creation: first created -> highest priority.
  # See how all your routes lay out with "rake routes".

  # You can have the root of your site routed with "root"
  get 'welcome/index'

  # Example of regular route:
  #   get 'products/:id' => 'catalog#view'

  # Example of named route that can be invoked with purchase_url(id: product.id)
  #   get 'products/:id/purchase' => 'catalog#purchase', as: :purchase

  # Example resource route (maps HTTP verbs to controller actions automatically):
  #   resources :products

  # Example resource route with options:
  #   resources :products do
  #     member do
  #       get 'short'
  #       post 'toggle'
  #     end
  #
  #     collection do
  #       get 'sold'
  #     end
  #   end

  # Example resource route with sub-resources:
  #   resources :products do
  #     resources :comments, :sales
  #     resource :seller
  #   end

  # Example resource route with more complex sub-resources:
  #   resources :products do
  #     resources :comments
  #     resources :sales do
  #       get 'recent', on: :collection
  #     end
  #   end

  # Example resource route with concerns:
  #   concern :toggleable do
  #     post 'toggle'
  #   end
  #   resources :posts, concerns: :toggleable
  #   resources :photos, concerns: :toggleable

  # Example resource route within a namespace:
  #   namespace :admin do
  #     # Directs /admin/products/* to Admin::ProductsController
  #     # (app/controllers/admin/products_controller.rb)
  #     resources :products
  #   end
end