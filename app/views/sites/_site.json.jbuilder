json.extract! site, :id, :name, :address, :districs, :city, :postalcode, :user_id, :created_at, :updated_at, :Long, :Lat
json.url site_url(site, format: :json)