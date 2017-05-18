class CreateCameras < ActiveRecord::Migration
  def change
    create_table :cameras do |t|
      t.string :name
      t.string :ip_address
      t.string :cam_type
      t.string :lense
      t.string :config
      t.string :other_info

      t.timestamps null: false
    end
  end
end
