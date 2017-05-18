class CreateUsersites < ActiveRecord::Migration
  def change
    create_table :usersites do |t|
      t.references :user, index: true, foreign_key: true
      t.references :site, index: true, foreign_key: true
      t.string :add
      t.string :edit
      t.string :remove

      t.timestamps null: false
    end
  end
end
