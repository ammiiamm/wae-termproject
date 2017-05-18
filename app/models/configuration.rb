class Configuration < ActiveRecord::Base
  belongs_to :site
  belongs_to :user
  belongs_to :camera
  belongs_to :zone
end
