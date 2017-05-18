json.extract! camera, :id, :name, :ip_address, :cam_type, :lense, :config, :other_info, :created_at, :updated_at
json.url camera_url(camera, format: :json)